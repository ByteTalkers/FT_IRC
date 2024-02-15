#pragma once
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <map>
#include <vector>

#define BACKLOG 10
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

class Server
{
  private:
    std::string m_password;
    int m_portnum;
    int m_serv_sock;
    int m_kqueue;
    std::map<int, Client> m_clients;         // 클라이언트 목록
    std::map<int, Channel> m_channels;       // 채널 목록
    std::vector<struct kevent> m_change_vec; // 이벤트 목록

  public:
    Server();
    Server(const Server &other);
    Server &operator=(const Server &other);
    ~Server();

    std::string getPassword();
    void setPassword(std::string pw);
    int getPortnum();
    void setPortnum(int number);

    Server(std::string s1, std::string s2);
    void checkPortnum(std::string str);
    void initServSock();
    void initKqueue();
    void handleKqueue();
    void handleConnect();
    void handleDisconnect();
	void handleTimeout();
    void handleSend(int fd);
    void handleRecv(int fd);
};