#ifndef Channel_HPP
#define Channel_HPP

#include "../Client/Client.hpp"
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <vector>

class Client; // 전방 선언

class Channel
{
  private:
    std::string m_name;
    time_t m_cretaed;
    std::vector<Client> m_operators;
    std::vector<Client> m_normals;
    std::vector<std::string> m_bans;
    std::map<std::string, bool> m_invitations; // 초대 받은 유저 리스트 목록
    std::string m_topic;
    std::string m_key;
    std::string m_password;
    int m_user_count;
    int m_limit_count;

    bool m_is_mode_invite;
    bool m_is_mode_key;
    bool m_is_mode_topic;
    bool m_is_mode_limit;

  public:
    Channel(std::string name, Client cl);
    Channel(const Channel &src);
    ~Channel(void);
    Channel &operator=(Channel const &rhs);

    void joinChannel(Client cl);
    void partChannel(Client cl);

    // 초대받은 유저 리스트 함수들
    void addInvitation(const std::string &user);
    bool isInvited(const std::string &user) const;
    void removeInvitation(const std::string &user);

    bool checkKey(const std::string &key);
    bool checkPassword(const std::string &password);

    // Getter
    std::string getName() const;
    time_t getCreated() const;
    std::vector<Client> getOperators() const;
    std::vector<Client> getNormals() const;
    std::vector<std::string> getBans() const;
    std::string getTopic() const;
    std::string getKey() const;
    int getUserCount() const;
    int getLimitCount() const;

    bool getModeInvite() const;
    bool getModeKey() const;
    bool getModeTopic() const;
    bool getModeLimit() const;

    // Setter
    void setName(std::string name);
    void setCreated(time_t created);
    void setOperators(std::vector<Client> opers);
    void setNormals(std::vector<Client> normals);
    void setBans(std::vector<std::string> bans);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void setUserCount(int count);
    void setLimitCount(int count);

    void setModeInvite(bool tf);
    void setModeKey(bool tf);
    void setModeTopic(bool tf);
    void setModeLimit(bool tf);
};

#endif