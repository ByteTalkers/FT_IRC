#include "Server.hpp"

/**
 * Checks if the given string ends with a carriage return and line feed ("\r\n").
 *
 * @param str The string to be checked.
 * @return True if the string ends with "\r\n", false otherwise.
 */
bool Server::checkBuffer(std::string str)
{
    if (str.find_last_of("\r\n") == str.length() - 1 && str.find_last_of("\r\n") != std::string::npos)
        return (true);
    else
        return (false);
}

/**
 * Adds a read event for a given socket file descriptor.
 *
 * This function adds a read event to the event vector for the server. The read event is used to monitor
 * the specified socket file descriptor for incoming data.
 *
 * @param sockfd The socket file descriptor to add the read event for.
 */
void Server::addReadEvent(int sockfd)
{
    struct kevent read_event;
    EV_SET(&read_event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(read_event);
}

/**
 * Adds a write event for the specified socket file descriptor.
 *
 * This function adds a write event to the event vector for the server. The write event
 * will be monitored by the event loop and trigger when the socket is ready for writing.
 *
 * @param sockfd The socket file descriptor to add the write event for.
 */
void Server::addWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * Disables the write event for the specified socket file descriptor.
 *
 * This function creates a kevent structure to disable the write event for the given socket file descriptor.
 * The kevent structure is then added to the change vector of the server.
 *
 * @param sockfd The socket file descriptor for which the write event needs to be disabled.
 */
void Server::disableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * Enables the write event for a given socket file descriptor.
 *
 * This function adds a write event to the event change vector of the server.
 * The write event will be monitored by the event loop and trigger when the socket
 * is ready for writing.
 *
 * @param sockfd The socket file descriptor for which to enable the write event.
 */
void Server::enableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * Enables multiple write events for a client in the server.
 *
 * This function enables write events for all clients in the same channel as the specified client.
 * If the write event type of the specified client is EVERYBUTME, the write event for the specified client is disabled.
 *
 * @param clnt The client for which to enable multiple write events.
 */
void Server::enableMultipleWrite(Client &clnt)
{
    writeEvent event_type = clnt.getWriteTypes();
    std::string ch_name = clnt.getCurChannel();

    // 채널 찾기
    Channel *ch = m_channels.at(ch_name);
    std::vector<Client *> normal = ch->getNormals();
    std::vector<Client *>::iterator iter;

    // 일단 해당채널 속 모든 클라이언트의 write 활성화
    for (iter = normal.begin(); iter != normal.end(); iter++)
        enableWriteEvent((*iter)->getsockfd());

    // 필요 시, 파라미터 clnt의 write만 해제
    if (event_type == EVERYBUTME)
        disableWriteEvent(clnt.getsockfd());
}