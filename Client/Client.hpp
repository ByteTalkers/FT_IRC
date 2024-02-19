#pragma once
#include "../Channel/Channel.hpp"
#include "../Message/Command.hpp"
#include "../Message/Message.hpp"
#include "../Server/Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

enum writeEvent
{
    NONE,
    MYSELF,
    EVERYBUTME,
    EVERYONE
};

class Channel; // 전방 선언
class Message;
class Server;

class Client
{
  private:
    int m_socket_fd;           // 소켓 fd
    std::string m_recv_data;   // 받은 데이터
    std::string m_send_msg;    // 보낼 메시지
    std::string m_nick;        // 닉네임
    std::string m_username;    // 유저네임
    std::string m_password;    // 패스워드
    std::string m_cur_channel; // 현재 채널
    int m_flag_connect;        // 연결 여부
    bool m_is_op;              // op(방장) 여부
    bool m_is_registered;      // 서버 등록 여부
    writeEvent m_write_types;  // write 이벤트의 종류

  public:
    Client();
    Client(const Client &other);
    Client &operator=(const Client &other);
    ~Client();

    // getter 함수들
    int getsockfd();
    std::string getNick();
    std::string getRecvData();
    std::string getSendMsg();
    std::string getUser();
    std::string getCurChannel();
    writeEvent getWriteTypes();
    bool getRegisterd();

    // Setter 함수들
    void setPassword(const std::string &password);
    void setNick(const std::string &nick);
    void setUsername(const std::string &username);
    void setRecvData(const char *data);
    void setWriteTypes(const writeEvent type);
    void setCurChannel(const std::string channel);

    void setSendMsg(std::string msg);
    void addSendMsg(std::string msg);
    void setRegistered(bool tf);

    void startListen(int serv_sock);
    void startParseMessage();
    void startResponse(std::map<int, Channel> &channels);
    void startSend();
};
