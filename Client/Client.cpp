#include "Client.hpp"

Client::Client() : m_is_registered(false), m_leave_msg("null")
{
    m_is_register_flags[PASS_REG] = false;
    m_is_register_flags[NICK_REG] = false;
    m_is_register_flags[USER_REG] = false;
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
        m_realname = other.m_realname;
        m_password = other.m_password;
        m_cur_channel = other.m_cur_channel;
        m_flag_connect = other.m_flag_connect;
        m_is_registered = other.m_is_registered;
        m_is_register_flags[0] = other.m_is_register_flags[0];
        m_is_register_flags[1] = other.m_is_register_flags[1];
        m_is_register_flags[2] = other.m_is_register_flags[2];
        m_write_types = other.m_write_types;
        m_leave_msg = other.m_leave_msg;
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

/**
 * @brief 클라이언트의 호스트 이름을 설정하는 함수입니다.
 *
 * @param clnt_adr 클라이언트의 소켓 주소 구조체입니다.
 */
void Client::setHostname(struct sockaddr_in &clnt_adr)
{
    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];

    getnameinfo((struct sockaddr *)&clnt_adr, sizeof(clnt_adr), hostname, NI_MAXHOST, service, NI_MAXSERV, 0);
    this->m_hostname = hostname;
}

void Client::setRealname(const std::string &realname)
{
    m_realname = realname;
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

std::string Client::getClientPrefix()
{
    return m_nick + "!" + m_username + "@" + m_hostname;
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

std::string Client::getRealname()
{
    return m_realname;
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

bool *Client::getIsRegisterFlags()
{
    return m_is_register_flags;
}

std::string Client::getLeaveMsg()
{
    return m_leave_msg;
}

/**
 * @brief 클라이언트의 연결 수락 및 비차단 처리를 수행하는 함수입니다.
 *
 * @param serv_sock 서버 소켓 디스크립터
 * @throws std::runtime_error accept() 함수 호출 중 오류가 발생한 경우
 */
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

/**
 * @brief 클라이언트의 메시지 파싱을 시작하는 함수입니다.
 *
 * @param serv 서버 객체에 대한 참조입니다.
 */
void Client::startParseMessage(Server &serv)
{
    // message 클래스의 객체 넣기
    Message msg(m_recv_data);

    msg.parsingOrigin();
    m_recv_data.clear();
    msg.execute(serv, *this);
}

/**
 * @brief 클라이언트의 데이터를 전송하는 함수입니다.
 *
 * 클라이언트 소켓을 통해 m_send_msg에 저장된 메시지를 전송합니다.
 * 전송 후 m_send_msg는 비워집니다.
 */
void Client::startSend()
{
    int clnt_sock = getsockfd();
    send(clnt_sock, m_send_msg.c_str(), m_send_msg.length(), 0);

    // m_send_msg 비워주기
    m_send_msg.clear();
}

void Client::addSendMsg(std::string msg)
{
    this->m_send_msg += msg;
}

void Client::setLeaveMsg(std::string msg)
{
    this->m_leave_msg = msg;
}