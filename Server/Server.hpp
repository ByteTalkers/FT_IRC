#pragma once
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"

#include <arpa/inet.h>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <map>
#include <utility>
#include <vector>

#define BACKLOG 10
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

class Client;
class Channel;

class Server
{
  private:
    std::string m_password;
    int m_portnum;
    int m_serv_sock;
    int m_kqueue;
    std::map<int, Client> m_clients;             // 클라이언트 목록
    std::map<std::string, Channel *> m_channels; // 채널 목록
    std::vector<struct kevent> m_change_vec;     // 이벤트 목록

    std::string m_name; // 서버 이름
    time_t m_created;   // 생성시간
    bool m_error;       // 에러 발생 여부

  public:
    Server();
    Server(const Server &other);
    Server &operator=(const Server &other);
    ~Server();

    std::string getPassword();
    void setPassword(std::string pw);
    int getPortnum();
    void setPortnum(int number);
    void setErrorCode();

    Server(std::string password);
    void checkPortnum(std::string str);
    void initServSock();
    void initKqueue();
    void handleKqueue();
    void handleConnect();
    void handleDisconnect(int fd);
    void handleSend(int fd);
    void handleRecv(int fd);
    int endServ();
    void addChannel(std::string channelName, Channel *channel);

    bool checkBuffer(std::string str);
    void addReadEvent(int sockfd);
    void addWriteEvent(int sockfd);
    void disableWriteEvent(int sockfd);
    void enableWriteEvent(int sockfd);
    void enableMultipleWrite(Client &clnt);

    std::string getName();
    time_t getCreated();
    std::string getClientCount();
    std::map<std::string, Channel *> &getChannels();

    void setName(std::string name);
    void setCreated(time_t time);

    Channel *findChannel(const std::string &ch_name);
    Client *findClient(const std::string &client_name);
    bool searchNick(int fd, const std::string &nick);
    void delEmptyChannel();
    void delClientFromChannel(Client &clnt);
};