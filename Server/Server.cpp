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
 * Checks if the given string represents a valid port number and sets it as the port number for the server.
 *
 * @param str The string representing the port number.
 * @throws std::runtime_error if the string is not a valid port number.
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
 * Initializes the server socket.
 * This function creates a server socket, sets socket options, binds the socket to a specific address and port,
 * and starts listening for incoming connections.
 * @throws std::runtime_error if any error occurs during socket creation, option setting, binding, or listening.
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
 * Initializes the kqueue for the server.
 *
 * This function creates a kqueue and registers the server socket for read events.
 *
 * @throws std::runtime_error if kqueue creation fails.
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
 * Handles the kqueue events for the server.
 * This function continuously listens for events using kqueue and handles them accordingly.
 * It checks for events on the server socket, client connections, and data read/write events.
 * If an event indicates a new client connection, it calls the handleConnect function.
 * If an event indicates a client disconnection, it calls the handleDisconnect function.
 * If an event indicates data to be read, it calls the handleRecv function.
 * If an event indicates data to be sent, it calls the handleSend function.
 *
 * @throws std::runtime_error if there is an error with the server socket.
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
 * Handles the connection with a client.
 * This function starts listening for incoming data from the client socket,
 * adds read and write events for the client socket, disables the write event,
 * and registers the client in the server's client map.
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
 * @brief Handles the receiving of data from a client socket.
 *
 * This function receives data from the client socket specified by the file descriptor `fd`.
 * It reads the data into a buffer, checks the buffer for validity, and then performs data parsing.
 * If the buffer is valid, it sets the received data in the corresponding client object and triggers the appropriate
 * write events.
 *
 * @param fd The file descriptor of the client socket.
 * @throws std::runtime_error if an error occurs while receiving data.
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
 * @brief Handles the send operation for a client socket.
 *
 * This function is responsible for handling the send operation for a specific client socket.
 * It retrieves the client object associated with the given file descriptor and initiates the send operation.
 * After the send operation is completed, it disables the write event for the client socket and sets the write types to
 * NONE.
 *
 * @param fd The file descriptor of the client socket.
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
 * @brief Handles the disconnection of a client.
 *
 * This function is responsible for handling the disconnection of a client from the server.
 * It performs the following tasks:
 * 1. Removes the client from the client list.
 * 2. Removes the client from any channels they were a part of.
 * 3. Deletes any empty channels.
 * 4. Closes the client's socket.
 *
 * @param fd The file descriptor of the client's socket.
 * @throws std::runtime_error if an error occurs while closing the socket.
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
 * @brief The std::string class represents a sequence of characters.
 *
 * This class provides a convenient way to manipulate strings of characters.
 * It supports various operations such as concatenation, substring extraction,
 * and comparison. The string is stored as a sequence of characters in a dynamic
 * array, allowing it to grow or shrink as needed.
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
 * @brief Finds a channel by name.
 *
 * This function searches for a channel with the specified name in the server's list of channels.
 *
 * @param ch_name The name of the channel to find.
 * @return A pointer to the found channel, or NULL if the channel is not found.
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
 * @brief Finds a client with the specified name.
 *
 * @param client_name The name of the client to find.
 * @return A pointer to the found client, or NULL if not found.
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
 * @brief Searches for a client with the given nickname.
 *
 * This function searches for a client with the specified nickname among the connected clients.
 * It iterates through the map of clients and checks if the nickname matches the given nickname,
 * excluding the client with the specified file descriptor.
 *
 * @param fd The file descriptor of the client to exclude from the search.
 * @param nick The nickname to search for.
 * @return True if a client with the given nickname is found, false otherwise.
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
 * @brief Ends the server and cleans up allocated resources.
 *
 * This function iterates through the map of channels and deletes each channel object.
 * It is responsible for cleaning up any allocated resources before the server is terminated.
 *
 * @return The error code associated with the server.
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
 * @brief Adds a channel to the server.
 *
 * This function adds a channel to the server's collection of channels.
 * The channel is associated with a unique channel name.
 *
 * @param channelName The name of the channel to be added.
 * @param channel A pointer to the Channel object to be added.
 */
void Server::addChannel(std::string channelName, Channel *channel)
{
    m_channels.insert(std::make_pair(channelName, channel));
}

/**
 * @brief Deletes empty channels from the server.
 *
 * This function iterates through the list of channels and identifies the empty channels.
 * Empty channels are those with no users in them. The names of the empty channels are stored
 * in a separate list. Then, the function iterates through the empty channel list and deletes
 * the corresponding channels from the server.
 *
 * @note This function assumes that the `m_channels` member variable is a map of channel names
 *       to Channel objects, where each Channel object represents a channel on the server.
 *       The `getUserCount()` function is used to determine the number of users in a channel.
 *
 * @note This function does not delete the users in the empty channels. It only removes the
 *       empty channels from the server.
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
 * @brief Removes a client from all channels and sends a quit message to the remaining members.
 *
 * This function removes the specified client from all channels in the server and sends a quit message
 * to the remaining members of each channel. If the client's leave message is "null", a default quit
 * message with "Connection: close" is sent. Otherwise, the client's custom leave message is included
 * in the quit message.
 *
 * @param clnt The client to be removed from the channels.
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