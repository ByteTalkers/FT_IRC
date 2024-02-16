#include "Server.hpp"

Server::Server() : m_password("password"), m_portnum(9000)
{
}

Server::Server(const Server &other)
{
    *this = other;
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        m_password = other.m_password;
        m_portnum = other.m_portnum;
        m_serv_sock = other.m_serv_sock;
        m_kqueue = other.m_kqueue;
    }
    return *this;
}

Server::~Server()
{
}

int Server::getPortnum()
{
    return m_portnum;
}

void Server::setPortnum(int number)
{
    m_portnum = number;
}

std::string Server::getPassword()
{
    return m_password;
}

void Server::setPassword(std::string pw)
{
    m_password = pw;
}

Server::Server(std::string s1, std::string s2) : m_password(s2)
{
    checkPortnum(s1);
    initServSock();
    initKqueue();
    handleKqueue();
}

void Server::checkPortnum(std::string str)
{
    std::istringstream port_stream(str);
    int port_number;

    // 0~9가 아닌 것의 위치가 npos가 아니면 throw
    if (str.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error("not a valid port number");
    port_stream >> port_number;

    // port_number가 잘 알려진 포트(0 ~ 1023)이거나 65535 초과 시 throw
    if ((port_number >= 0 && port_number <= 1023) || port_number > 65535)
        throw std::runtime_error("not a valid port number");

    setPortnum(port_number);
}

void Server::initServSock()
{
    struct sockaddr_in serv_adr;
    int enable_reuse = 1;

    // 서버 소켓 생성
    if ((m_serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("socket() error");

    // bind() 오류 해결
    if (setsockopt(m_serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0)
    {
        close(m_serv_sock);
        throw std::runtime_error("setsockopt() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;                // ipv4로 설정
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // ip 주소 (사용가능한 랜카드 ip주소 아무거나)
    serv_adr.sin_port = htons(m_portnum);         // 포트번호 잡기

    if (bind(m_serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        close(m_serv_sock);
        throw std::runtime_error("bind() error");
    }

    if (listen(m_serv_sock, BACKLOG) == -1) // 연결요청 대기 큐 : 10개.
    {
        close(m_serv_sock);
        throw std::runtime_error("listen() error");
    }

    if (fcntl(m_serv_sock, F_SETFL, O_NONBLOCK)) // nonblock 처리
        throw std::runtime_error("fcntl() error");
}

void Server::initKqueue()
{
    // kq 만들기
    if ((m_kqueue = kqueue()) == -1)
    {
        close(m_serv_sock);
        throw std::runtime_error("kqueue() error");
    }

    // serv_sock의 read를 이벤트벡터에 등록
    struct kevent serv_event;
    EV_SET(&serv_event, m_serv_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    m_change_vec.push_back(serv_event);
}

void Server::handleKqueue()
{
    while (1)
    {
        struct kevent event_arr[MAX_EVENTS];
        // struct timespec timeout = {1, 0};
        int event_cnt = kevent(m_kqueue, &m_change_vec[0], m_change_vec.size(), event_arr, MAX_EVENTS, NULL);
        if (event_cnt == -1)
            break;
        m_change_vec.clear();

        for (int i = 0; i < event_cnt; i++)
        {
            int fd = event_arr[i].ident;

            if (fd == m_serv_sock)
            {
                if (event_arr[i].flags & EV_EOF)
                    throw std::runtime_error("server socket error");
                else
                    Server::handleConnect();
            }
            if (event_arr[i].flags & EV_EOF) // 연결이 끊어진 경우
                Server::handleDisconnect();
            else if (event_arr[i].filter == EVFILT_READ) // 읽기 이벤트 -> 읽고 파싱
                Server::handleRecv(fd);
            else if (event_arr[i].filter == EVFILT_WRITE) // 쓰기 이벤트 -> 데이터 전송
                Server::handleSend(fd);
            // else if (event_arr[i].filter == EVFILT_TIMER)
            // 	Server::handleTimeout();
        }
    }
}

void Server::handleConnect()
{
    Client clnt;
    clnt.startListen(this->m_serv_sock);

    struct kevent clnt_read_event;
    struct kevent clnt_write_event;
    EV_SET(&clnt_read_event, clnt.getsockfd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    EV_SET(&clnt_write_event, clnt.getsockfd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);

    m_change_vec.push_back(clnt_read_event);
    m_change_vec.push_back(clnt_write_event);

    // 클라이언트의 map에 등록
    m_clients[clnt.getsockfd()] = clnt;
}

void Server::handleRecv(int fd)
{
    std::map<int, Client>::iterator iter;
    for (iter = m_clients.begin(); iter != m_clients.end(); iter++)
    {
        if (iter->first == fd)
            break;
    }

    // 데이터 받기
    int clnt_sock = iter->first;
    Client &clnt = iter->second;
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = recv(clnt_sock, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes_read] = '\0';
    // std::cout << buffer << std::endl;
    clnt.setRecvData(buffer);

    // 추후 추가 : 데이터 체크 및 파싱
    // clnt.startParseMessage();

    // 추후 추가 : 데이터에 대한 응답 생성
    // clnt.startResponse(m_channels);
}

void Server::handleSend(int fd)
{
    std::map<int, Client>::iterator iter;
    for (iter = m_clients.begin(); iter != m_clients.end(); iter++)
    {
        if (iter->first == fd)
            break;
    }

    // 추후 추가 : 데이터 재전송
    // Client clnt = iter->second;
    // clnt.startSend();
}

void Server::handleDisconnect()
{
    exit(EXIT_SUCCESS);
}

std::string Server::getClientCount()
{
    std::stringstream ss;
    ss << m_clients.size();
    std::string ret = ss.str();
    return ret;
}

std::string Server::getName()
{
    return m_name;
}

time_t Server::getCreated()
{
    return m_created;
}

std::map<std::string, Channel *> &Server::getChannels() // Channel 포인터로 처리하기
{
    return m_channels;
}

void Server::setName(std::string name)
{
    m_name = name;
}

void Server::setCreated(time_t time)
{
    m_created = time;
}

void Server::handleTimeout()
{
}
