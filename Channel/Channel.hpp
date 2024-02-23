#ifndef Channel_HPP
#define Channel_HPP

#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <map>
#include <string>
#include <vector>

class Client; // 전방 선언
class Server;

class Channel
{
  private:
    std::string m_name; // 현재 채널의 이름
    time_t m_cretaed;
    std::vector<Client *> m_operators; // op인 유저들의 목록
    std::vector<Client *> m_normals;   // 채팅방 속 모든 유저들의 목록
    std::vector<std::string> m_bans;
    std::deque<std::string> m_invitations; // 초대 받은 유저 리스트 목록
    std::string m_topic;
    std::string m_key;
    std::string m_password;
    int m_limit_count;

    bool m_is_mode_invite;
    bool m_is_mode_key;
    bool m_is_mode_topic;
    bool m_is_mode_limit;

    bool m_is_topic_exist; // 토픽 세팅 여부

  public:
    Channel(std::string name, Client *cl);
    Channel(const Channel &src);
    ~Channel(void);
    Channel &operator=(Channel const &rhs);

    void partChannel(Client &cl);

    bool checkOp(Client &cl);
    // 초대받은 유저 리스트 함수들
    void addInvitation(const std::string &user);
    bool isInvited(const std::string &user) const;
    void removeInvitation(const std::string &user);

    bool checkKey(const std::string &key);
    bool checkPassword(const std::string &password);
    bool isMember(Client &cl) const;
    void addMember(Client *cl);
    bool isMemberNick(std::string &nick) const;
    bool isMemberNick(const std::string &nick) const;

    void addOperator(const std::string &nick);
    void popOperator(const std::string &nick);
    bool checkOpNick(const std::string &nick) const;

    // Getter
    std::string getName() const;
    time_t getCreated() const;
    std::vector<Client *> getOperators() const;
    std::vector<Client *> getNormals() const;
    std::string getTopic() const;
    std::string getKey() const;
    int getUserCount() const;
    int getLimitCount() const;

    bool getModeInvite() const;
    bool getModeKey() const;
    bool getModeTopic() const;
    bool getModeLimit() const;
    bool getTopicExist() const;

    // Setter
    void setName(std::string name);
    void setCreated(time_t created);
    void setOperators(std::vector<Client *> opers);
    void setNormals(std::vector<Client *> normals);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void setLimitCount(int count);
    void setModeInvite(bool tf);
    void setModeKey(bool tf);
    void setModeTopic(bool tf);
    void setModeLimit(bool tf);
    void setTopicExist(bool tf);

    // 기타 함수
    void addSendMsgAll(Server &server, const std::string &from, const std::string &cmd, const std::string &msg);
};

#endif