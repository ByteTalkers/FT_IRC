#pragma once
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
  private:
    int m_socket_fd;         // 소켓 fd
    std::string m_recv_data; // 받은 데이터

    std::string m_nick;        // 닉네임
    std::string m_username;    // 유저네임
    std::string m_password;    // 패스워드
    std::string m_cur_channel; // 현재 채널명
    int m_flag_connect;        // 연결 여부
    bool m_is_op;              // op(방장) 여부

  public:
    Client();
    Client &operator=(const Client &other);
    Client(const Client &other);
    ~Client();

    int getsockfd();
    void setRecvData(const char *data);
    std::string getRecvData();
    void startListen(int serv_sock);
<<<<<<< HEAD
    void setPassword(const std::string &password);
    void setNick(const std::string &nick);
    void setUsername(const std::string &username);
=======

    std::string getNick();
>>>>>>> 73035439b029db57e4f9ad17a30b99f10896e5c3
};