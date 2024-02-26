#pragma once
#include "../Channel/Channel.hpp"
#include "../Message/Command.hpp"
#include "../Message/Message.hpp"
#include "../Server/Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

enum eRegister
{
    PASS_REG,
    NICK_REG,
    USER_REG
};

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
    int m_socket_fd;           // 자기 자신의 소켓 fd
    int m_recv_fd;             // 송신할 클라이언트 fd
    std::string m_recv_data;   // 받은 데이터
    std::string m_send_msg;    // 보낼 메시지
    std::string m_nick;        // 닉네임
    std::string m_username;    // 유저네임
    std::string m_hostname;    // 호스트네임
    std::string m_realname;    // 리얼네임(등록 시 인자로 받음)
    std::string m_password;    // 패스워드
    std::string m_cur_channel; // 현재 채널
    int m_flag_connect;        // 연결 여부
    bool m_is_registered;      // 서버 등록 여부
    writeEvent m_write_types;  // write 이벤트의 종류
    std::string m_leave_msg;   // 떠나는 이유 관련 메시지

    bool m_is_register_flags[3]; // PASS, NICK, USER 등록 플래그

  public:
    Client();
    Client(const Client &other);
    Client &operator=(const Client &other);
    ~Client();

    // getter 함수들
    int getsockfd();
    int getrecvfd();
    std::string getNick() const;
    std::string getRecvData();
    std::string getSendMsg();
    std::string getUser();
    std::string getCurChannel();
    writeEvent getWriteTypes();
    std::string getHostname();
    std::string getRealname();
    bool getRegisterd();
    bool *getIsRegisterFlags();
    std::string getLeaveMsg();

    // Setter 함수들
    void setRecvFd(const int number);
    void setPassword(const std::string &password);
    void setNick(const std::string &nick);
    void setUsername(const std::string &username);
    void setHostname(struct sockaddr_in &clnt_adr);
    void setRealname(const std::string &realname);
    void setRecvData(const char *data);
    void setWriteTypes(const writeEvent type);
    void setCurChannel(const std::string channel);

    void setSendMsg(std::string msg);
    void addSendMsg(std::string msg);
    void setRegistered(bool tf);
    void setRegisterFlags(int i, bool tf);
    void setLeaveMsg(std::string msg);

    void startListen(int serv_sock);
    void startParseMessage(Server &serv);
    void startResponse(std::map<int, Channel> &channels);
    void startSend();
};
