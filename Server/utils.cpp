#include "Server.hpp"

void Server::addReadEvent(int sockfd)
{
    struct kevent read_event;
    EV_SET(&read_event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if (kevent(m_kqueue, &read_event, 1, NULL, 0, 0) == -1)
        throw std::runtime_error("kevent() of addReadEvent was wrong..");
}

void Server::addWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);

    if (kevent(m_kqueue, &write_event, 1, NULL, 0, 0) == -1)
        throw std::runtime_error("kevent() of addWriteEvent was wrong..");
}

void Server::disableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);

    if (kevent(m_kqueue, &write_event, 1, NULL, 0, 0) == -1)
        throw std::runtime_error("kevent() of disableWriteEvent was wrong..");
}

void Server::enableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);

    if (kevent(m_kqueue, &write_event, 1, NULL, 0, 0) == -1)
        throw std::runtime_error("kevent() of enableWriteEvent was wrong..");
}
