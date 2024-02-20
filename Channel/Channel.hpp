#ifndef Channel_HPP
#define Channel_HPP

#include "../Client/Client.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

class Client; // 전방 선언

class Channel
{
  private:
    std::string m_name;				// 현재 채널의 이름
    time_t m_cretaed;
    std::vector<Client> m_operators; // op인 유저들의 목록
    std::vector<Client> m_normals;   // 채팅방 속 모든 유저들의 목록
    std::vector<std::string> m_bans;
    std::vector<Client> m_invited;
    std::string m_topic;
    std::string m_key;
    int m_user_count;
    int m_limit_count;

    bool m_is_mode_invite;
    bool m_is_mode_key;
    bool m_is_mode_topic;
    bool m_is_mode_limit;

    bool m_is_set_topic; // 토픽 세팅 여부

  public:
    Channel(std::string name, Client cl);
    Channel(const Channel &src);
    ~Channel(void);
    Channel &operator=(Channel const &rhs);

    void joinChannel(Client cl);
    void partChannel(Client cl);

    bool checkOp(Client cl);

    // Getter
    std::string getName() const;
    time_t getCreated() const;
    std::vector<Client> getOperators() const;
    std::vector<Client> getNormals() const;
    std::vector<Client> getInvited() const;
    std::string getTopic() const;
    std::string getKey() const;
    int getUserCount() const;
    int getLimitCount() const;

    bool getModeInvite() const;
    bool getModeKey() const;
    bool getModeTopic() const;
    bool getModeLimit() const;

    bool getSetTopic() const;

    // Setter
    void setName(std::string name);
    void setCreated(time_t created);
    void setOperators(std::vector<Client> opers);
    void setNormals(std::vector<Client> normals);
    void setInvited(std::vector<Client> invited);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void setUserCount(int count);
    void setLimitCount(int count);

    void setModeInvite(bool tf);
    void setModeKey(bool tf);
    void setModeTopic(bool tf);
    void setModeLimit(bool tf);

    void setSetTopic(bool tf);

    void addSendMsgAll(const std::string &from, const std::string &cmd, const std::string &msg);

    bool checkClientIn(Client &cl);
};

#endif