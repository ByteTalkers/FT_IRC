#include "Server.hpp"

bool Server::checkBuffer(std::string str)
{
    if (str.find_last_of("\r\n") == str.length() - 1 && str.find_last_of("\r\n") != std::string::npos)
        return (true);
    else
        return (false);
}

void Server::addReadEvent(int sockfd)
{
    struct kevent read_event;
    EV_SET(&read_event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(read_event);
}

void Server::addWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

void Server::disableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

void Server::enableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

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