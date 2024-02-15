#pragma once
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
#include <ctime>

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
    std::vector<struct kevent> m_change_vec; // 이벤트 목록

    std::string m_name;     // 서버 이름
    time_t m_created;       // 생성시간

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
    void handleSend(int fd);
    void handleRecv(int fd);

    std::string getName();
    time_t getCreated();
    std::string getClientCount();

    void setName(std::string name);
    void setCreated(time_t time);
};