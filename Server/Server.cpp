#include "Server.hpp"

Server::Server() : m_password("password"), m_portnum(9000), m_name("ByteTalkers"), m_error(false)
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
        m_clients = other.m_clients;
        m_channels = other.m_channels;
        m_change_vec = other.m_change_vec;
        m_name = other.m_name;
        m_created = other.m_created;
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

void Server::setErrorCode()
{
    m_error = true;
}

Server::Server(std::string password) : m_password(password), m_name("ByteTalkers"), m_error(false)
{
}

/**
 * @brief 포트 번호를 확인하고 설정하는 함수입니다.
 *
 * @param str 포트 번호를 나타내는 문자열입니다.
 * @throws std::runtime_error 유효하지 않은 포트 번호일 경우 예외를 던집니다.
 */
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

/**
 * @brief 서버 소켓을 초기화하는 함수입니다.
 *
 * @details 서버 소켓을 생성하고 bind, listen 등의 작업을 수행합니다.
 *          소켓 생성에 실패하거나 오류가 발생한 경우 예외를 던집니다.
 *
 * @throws std::runtime_error 소켓 생성, bind, listen, setsockopt, fcntl 함수에서 오류가 발생한 경우
 */
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

/**
 * @brief Server 클래스의 initKqueue 함수입니다.
 *
 * 이 함수는 kqueue를 초기화하고, serv_sock의 read 이벤트를 이벤트 벡터에 등록합니다.
 *
 * @throws std::runtime_error kqueue() 함수 호출에 실패한 경우 예외를 던집니다.
 */
void Server::initKqueue()
{
    // kq 만들기
    if ((m_kqueue = kqueue()) == -1)
    {
        close(m_serv_sock);
        throw std::runtime_error("kqueue() error");
    }

    // serv_sock의 read를 이벤트벡터에 등록
    addReadEvent(m_serv_sock);
}

/**
 * @brief kqueue를 처리하는 함수입니다.
 *
 * 이 함수는 무한 루프를 돌면서 kqueue 이벤트를 처리합니다.
 * kqueue에서 발생한 이벤트를 처리하고, 연결을 끊거나 데이터를 읽고 쓰는 작업을 수행합니다.
 *
 * @throws std::runtime_error 서버 소켓 에러가 발생한 경우 예외를 던집니다.
 */
void Server::handleKqueue()
{
    while (1)
    {
        struct kevent event_arr[MAX_EVENTS];
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
                continue;
            }
            if (event_arr[i].flags & EV_EOF) // 연결을 끊으려고 하는 경우
                Server::handleDisconnect(fd);
            else if (event_arr[i].filter == EVFILT_READ) // 읽기 이벤트 -> 읽고 파싱
                Server::handleRecv(fd);
            else if (event_arr[i].filter == EVFILT_WRITE) // 쓰기 이벤트 -> 데이터 전송
                Server::handleSend(fd);
        }
    }
}

/**
 * @brief 클라이언트 연결을 처리하는 함수입니다.
 *
 * 클라이언트 소켓을 생성하고, 이벤트를 활성화시키고, 클라이언트를 서버에 등록합니다.
 */
void Server::handleConnect()
{
    Client clnt;
    clnt.startListen(this->m_serv_sock);

    // 클라이언트 소켓의 read, write 이벤트 활성화
    addReadEvent(clnt.getsockfd());
    addWriteEvent(clnt.getsockfd());

    // 클라이언트 소켓의 write 이벤트 비활성화
    disableWriteEvent(clnt.getsockfd());
    clnt.setWriteTypes(NONE);

    // 클라이언트의 map에 등록
    m_clients.insert(std::make_pair(clnt.getsockfd(), clnt));
}

/**
 * @brief 클라이언트로부터 데이터를 수신하고 처리하는 함수입니다.
 *
 * @param fd 파일 디스크립터
 */
void Server::handleRecv(int fd)
{
    // 데이터 받기
    int clnt_sock = fd;
    Client &clnt = m_clients.at(fd);
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = recv(clnt_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read == -1)
        throw std::runtime_error("something is wrong on clnt_sock or recv()");
    if (checkBuffer(buffer) == true)
    {
        buffer[bytes_read] = '\0';
        clnt.setRecvData(buffer);

        // 데이터 파싱
        std::cout << "================ start ==========\n";
        std::cout << "clnt: " << clnt.getRecvData() << std::endl;
        std::cout << "read success" << std::endl;
        clnt.startParseMessage(*this);

        // write 이벤트 활성화
        if (clnt.getWriteTypes() == MYSELF) // 서버 -> 클라이언트 자기 자신
            enableWriteEvent(clnt_sock);
        else if (clnt.getWriteTypes() == EVERYBUTME || clnt.getWriteTypes() == EVERYONE) // 서버 -> 모든 클라이언트
            enableMultipleWrite(clnt);
    }
    else
        clnt.setRecvData(buffer);
}

/**
 * @brief 클라이언트 소켓에 데이터를 전송하는 함수입니다.
 *
 * @param fd 전송할 클라이언트 소켓의 파일 디스크립터
 */
void Server::handleSend(int fd)
{
    // clnt 찾기 (at()을 써야 이미 있는 원소가 찾아진다.)
    Client &clnt = m_clients.at(fd);

    // 추후 추가 : 데이터 재전송
    clnt.startSend();
    std::cout << "write success: fd => " << clnt.getsockfd() << std::endl;
    std::cout << "================ end ==========\n";

    // 클라이언트 소켓의 write 이벤트 비활성화
    disableWriteEvent(clnt.getsockfd());
    clnt.setWriteTypes(NONE);
}

/**
 * @brief 클라이언트의 연결이 끊겼을 때 호출되는 함수입니다.
 *
 * @param fd 연결이 끊긴 클라이언트의 파일 디스크립터
 */
void Server::handleDisconnect(int fd)
{
    std::cout << "============ Disconnection start =========" << std::endl;
    // 클라이언트 목록에서 해당하는 클라이언트를 fd로 찾는다.
    std::map<int, Client>::iterator it_clnt = m_clients.find(fd);
    Client clnt = it_clnt->second;

    // 각 채널에서 나갈 클라이언트 삭제
    delClientFromChannel(clnt);

    // 빈 채널 삭제 (ctrl + c를 위한 로직)
    delEmptyChannel();

    // 클라이언트의 소켓을 닫는다.
    if (close(fd) == -1)
        throw std::runtime_error("close() error");

    // 전체 유저 목록에서 지운다.
    m_clients.erase(it_clnt);
    std::cout << "success: erase" << std::endl;
    std::cout << "========== Disconnection end =======" << std::endl;
}

/**
 * @brief 클라이언트 수를 문자열 형태로 반환하는 함수입니다.
 *
 * @return 클라이언트 수를 나타내는 문자열
 */
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

std::map<std::string, Channel *> &Server::getChannels()
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

/**
 * @brief 채널을 찾는 함수입니다.
 *
 * @param ch_name 찾을 채널의 이름
 * @return 찾은 채널의 포인터. 채널을 찾지 못한 경우 NULL을 반환합니다.
 */
Channel *Server::findChannel(const std::string &ch_name)
{
    try
    {
        this->m_channels.find(ch_name);
        return this->m_channels.at(ch_name);
    }
    catch (const std::exception &e)
    {
        return NULL;
    }
}

/**
 * @brief 클라이언트를 찾는 함수입니다.
 *
 * @param client_name 찾을 클라이언트의 이름
 * @return Client* 찾은 클라이언트의 포인터, 찾지 못한 경우 NULL을 반환합니다.
 */
Client *Server::findClient(const std::string &client_name)
{
    std::map<int, Client>::iterator it;
    for (it = this->m_clients.begin(); it != this->m_clients.end(); it++)
    {
        if ((*it).second.getNick() == client_name)
        {
            return &(*it).second;
        }
    }
    return NULL;
}

/**
 * @brief 지정된 닉네임을 가진 클라이언트를 검색합니다.
 *
 * @param fd 클라이언트의 파일 디스크립터
 * @param nick 검색할 닉네임
 * @return true 클라이언트를 찾았을 경우
 * @return false 클라이언트를 찾지 못한 경우
 */
bool Server::searchNick(int fd, const std::string &nick)
{
    std::map<int, Client>::iterator it;
    for (it = this->m_clients.begin(); it != this->m_clients.end(); it++)
    {
        if ((*it).first != fd && (*it).second.getNick() == nick)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 서버를 종료하는 함수입니다.
 *
 * 이 함수는 서버에서 사용 중인 모든 채널을 삭제하고, 서버의 에러 코드를 반환합니다.
 *
 * @return 서버의 에러 코드를 반환합니다.
 */
int Server::endServ()
{
    std::map<std::string, Channel *>::iterator it_map;
    for (it_map = m_channels.begin(); it_map != m_channels.end(); it_map++)
    {
        if (it_map->second)
            delete it_map->second;
    }

    return static_cast<int>(m_error);
}

/**
 * @brief 채널을 추가하는 함수입니다.
 *
 * @param channelName 추가할 채널의 이름
 * @param channel 추가할 채널의 포인터
 */
void Server::addChannel(std::string channelName, Channel *channel)
{
    m_channels.insert(std::make_pair(channelName, channel));
}

/**
 * @brief 빈 채널을 삭제하는 함수입니다.
 *
 * 채널 목록을 돌면서 빈 채널을 찾고, 해당 채널을 삭제합니다.
 *
 * @return 없음
 */
void Server::delEmptyChannel()
{
    // 빈 채널 목록 생성
    std::map<std::string, Channel *>::iterator it_ch;
    std::vector<std::string> empty_list;

    // 채널의 목록을 돌면서 빈 채널을 찾고, 그 이름을 empty_list에 넣는다.
    for (it_ch = m_channels.begin(); it_ch != m_channels.end(); ++it_ch)
    {
        if (it_ch->second->getUserCount() == 0)
            empty_list.push_back(it_ch->first);
    }

    // empty_list를 돌리면서 빈 채널 지우기
    for (size_t i = 0; i < empty_list.size(); i++)
    {
        std::map<std::string, Channel *>::iterator it;
        it = m_channels.find(empty_list[i]);

        if (it != m_channels.end())
        {
            delete it->second;
            m_channels.erase(it);
        }
    }
}

/**
 * @brief 클라이언트를 채널에서 제거하는 함수입니다.
 *
 * @param clnt 제거할 클라이언트 객체
 */
void Server::delClientFromChannel(Client &clnt)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = m_channels.begin(); it != m_channels.end(); ++it)
    {
        Channel *ch = it->second;
        if (ch->isMember(clnt))
        {
            // 이때 채널에 있는 모든 유저들에게 메시지를 보내야 한다.
            if (clnt.getLeaveMsg() == "null")
                ch->addSendMsgAll(*this, clnt.getClientPrefix(), "QUIT", "", "Connection: close");
            else
                ch->addSendMsgAll(*this, clnt.getClientPrefix(), "QUIT", "", "Quit: " + clnt.getLeaveMsg());
            ch->partChannel(clnt);
        }
    }
}