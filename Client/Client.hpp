#pragma once
#include "../Channel/Channel.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

class Channel; // 전방 선언

class Client
{
  private:
    int m_socket_fd;         // 소켓 fd
    std::string m_recv_data; // 받은 데이터
    std::string m_send_msg;  // 보낼 메시지

    std::string m_nick;        // 닉네임
    std::string m_username;    // 유저네임
    std::string m_cur_channel; // 현재 채널명
    int m_flag_connect;        // 연결 여부
    bool m_is_op;              // op(방장) 여부

  public:
    Client();
    Client &operator=(const Client &other);
    Client(const Client &other);
    ~Client();

    int getsockfd();
    std::string getNick();
    std::string getRecvData();
    void setRecvData(const char *data);

    void startListen(int serv_sock);
    void startParseMessage();
    void startResponse(std::map<int, Channel> &channels);
    void startSend();
};