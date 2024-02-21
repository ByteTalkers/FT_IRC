#include "Client.hpp"

Client::Client()
{
}

Client::Client(const Client &other)
{
    *this = other;
}

Client &Client::operator=(const Client &other)
{
    if (this != &other)
    {
        m_socket_fd = other.m_socket_fd;
        m_recv_data = other.m_recv_data;
        m_send_msg = other.m_send_msg;
        m_nick = other.m_nick;
        m_username = other.m_username;
        m_hostname = other.m_hostname;
        m_password = other.m_password;
        m_cur_channel = other.m_cur_channel;
        m_flag_connect = other.m_flag_connect;
        m_is_op = other.m_is_op;
        m_is_registered = other.m_is_registered;
        m_write_types = other.m_write_types;
        // 추가된 멤버 변수가 있다면 여기에 복사 로직을 추가
    }
    return *this;
}

Client::~Client()
{
}

int Client::getsockfd()
{
    return m_socket_fd;
}

// Setter 함수들
void Client::setRecvFd(const int number)
{
    m_recv_fd = number;
}

void Client::setPassword(const std::string &password)
{
    m_password = password;
}

void Client::setNick(const std::string &nick)
{
    m_nick = nick;
}

void Client::setUsername(const std::string &username)
{
    m_username = username;
}

void Client::setHostname(struct sockaddr_in &clnt_adr)
{
    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];

    getnameinfo((struct sockaddr *)&clnt_adr, sizeof(clnt_adr), hostname, NI_MAXHOST, service, NI_MAXSERV, 0);
    this->m_hostname = hostname;
}

void Client::setRecvData(const char *data)
{
    std::string newstr(data);
    m_recv_data += newstr;
}

void Client::setSendMsg(std::string msg)
{
    m_send_msg = msg;
}

void Client::setWriteTypes(const writeEvent type)
{
    m_write_types = type;
}

void Client::setRegistered(bool tf)
{
    m_is_registered = tf;
}

void Client::setCurChannel(const std::string channel)
{
    m_cur_channel = channel;
}

void Client::setRegisterFlags(int i, bool tf)
{
    m_is_register_flags[i] = tf;
}


// Getter 함수들
int Client::getrecvfd()
{
    return m_recv_fd;
}

std::string Client::getNick() const
{
    return m_nick;
}

std::string Client::getUser()
{
    return m_username;
}

std::string Client::getHostname()
{
    return m_hostname;
}

std::string Client::getRecvData()
{
    return m_recv_data;
}

std::string Client::getSendMsg()
{
    return m_send_msg;
}

std::string Client::getCurChannel()
{
    return m_cur_channel;
}

writeEvent Client::getWriteTypes()
{
    return m_write_types;
}

bool Client::getRegisterd()
{
    return m_is_registered;
}

bool Client::getIsOp()
{
    return m_is_op;
}

bool *Client::getIsRegisterFlags()
{
    return m_is_register_flags;
}

void Client::startListen(int serv_sock)
{
    struct sockaddr_in clnt_adr;
    socklen_t clnt_len = sizeof(clnt_adr);

    // accept()
    m_socket_fd = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_len);
    if (m_socket_fd < 0)
        throw std::runtime_error("accept() error, client side");

    // nonblock 처리
    fcntl(m_socket_fd, F_SETFL, O_NONBLOCK);

    // setHostname()
    setHostname(clnt_adr);
}

void Client::startParseMessage(Server &serv)
{
    // message 클래스의 객체 넣기
    Message msg(m_recv_data);

    msg.parsingOrigin();
    m_recv_data.clear();
    msg.execute(serv, *this);
}

void Client::startResponse(std::map<int, Channel> &channels)
{
    // response 클래스의 객체 넣기
    (void)channels;
}

void Client::startSend()
{
    int clnt_sock = getsockfd();
    std::cout << "m_send_msg : " << m_send_msg << std::endl;
    send(clnt_sock, m_send_msg.c_str(), m_send_msg.length(), 0);

    std::cout << "handle send : " << getSendMsg() << std::endl;

    // m_send_msg 비워주기
    m_send_msg.clear();
}

void Client::addSendMsg(std::string msg)
{
    this->m_send_msg += msg;
}