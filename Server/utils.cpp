#include "Server.hpp"

/**
 * @brief 입력된 문자열이 "\r\n"으로 끝나는지 확인하는 함수입니다.
 *
 * @param str 확인할 문자열
 * @return 문자열이 "\r\n"으로 끝나면 true, 그렇지 않으면 false를 반환합니다.
 */
bool Server::checkBuffer(std::string str)
{
    if (str.find_last_of("\r\n") == str.length() - 1 && str.find_last_of("\r\n") != std::string::npos)
        return (true);
    else
        return (false);
}

/**
 * @brief 소켓에 대한 읽기 이벤트를 추가합니다.
 *
 * @param sockfd 소켓 파일 디스크립터
 */
void Server::addReadEvent(int sockfd)
{
    struct kevent read_event;
    EV_SET(&read_event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(read_event);
}

/**
 * @brief 소켓에 대한 쓰기 이벤트를 추가합니다.
 *
 * @param sockfd 소켓 파일 디스크립터
 */
void Server::addWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * @brief 소켓의 쓰기 이벤트를 비활성화하는 함수입니다.
 *
 * @param sockfd 비활성화할 소켓의 파일 디스크립터
 */
void Server::disableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * @brief 소켓에 대한 쓰기 이벤트를 활성화합니다.
 *
 * @param sockfd 활성화할 소켓의 파일 디스크립터
 */
void Server::enableWriteEvent(int sockfd)
{
    struct kevent write_event;
    EV_SET(&write_event, sockfd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
    m_change_vec.push_back(write_event);
}

/**
 * @brief 클라이언트의 쓰기 이벤트를 활성화하는 함수입니다.
 *
 * @param clnt 쓰기 이벤트를 활성화할 클라이언트 객체
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