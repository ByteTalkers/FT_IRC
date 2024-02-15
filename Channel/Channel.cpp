#include "Channel.hpp"

Channel::Channel(std::string name, Client cl) : m_name(name), m_user_count(1)
{
    this->m_operators.push_back(cl);
    this->m_cretaed = time(NULL);
}

Channel::~Channel()
{
    this->m_operators.clear();
    this->m_normals.clear();
    this->m_bans.clear();
}

// 채널에 들어옴
void Channel::joinChannel(Client cl) {
    if (this->m_is_mode_invite) {
        // todo: 초대 여부 확인
    }
    if (this->m_is_mode_key) {
        // todo: 비밀번호 입력 확인
    }
    if (this->m_is_mode_limit) {
        if (this->m_user_count == this->m_limit_count) {
            // todo: error reply 설정
        }
    } 

    this->m_user_count++;
    this->m_normals.push_back(cl);
}

// 채널 나가기
void Channel::partChannel(Client cl) {
    this->m_user_count--;
    for (std::size_t i = 0; i < this->m_operators.size(); i++) {
        if (this->m_operators[i].getNick() == cl.getNick()) {
            this->m_operators.erase(this->m_operators.begin() + i);
            return ;
        }
    }
    for (std::size_t i = 0; i < this->m_normals.size(); i++) {
        if (this->m_normals[i].getNick() == cl.getNick()) {
            this->m_normals.erase(this->m_normals.begin() + i);
            return ;
        }
    }
}

// Getter
std::string Channel::getName() const
{
    return this->m_name;
}
time_t Channel::getCreated() const
{
    return this->m_cretaed;
}
std::vector<Client> Channel::getOperators() const
{
    return this->m_operators;
}
std::vector<Client> Channel::getNormals() const
{
    return this->m_normals;
}
std::vector<std::string> Channel::getBans() const
{
    return this->m_bans;
}
std::string Channel::getTopic() const
{
    return this->m_topic;
}
std::string Channel::getKey() const
{
    return this->m_key;
}
int Channel::getUserCount() const
{
    return this->m_user_count;
}
int Channel::getLimitCount() const
{
    return this->m_limit_count;
}

bool Channel::getModeInvite() const
{
    return this->m_is_mode_invite;
}
bool Channel::getModeKey() const
{
    return this->m_is_mode_key;
}
bool Channel::getModeTopic() const
{
    return this->m_is_mode_topic;
}
bool Channel::getModeLimit() const
{
    return this->m_is_mode_limit;
}

// Setter
void Channel::setName(std::string name)
{
    this->m_name = name;
}
void Channel::setCreated(time_t created)
{
    this->m_cretaed = created;
}
void Channel::setOperators(std::vector<Client> opers)
{
    this->m_operators = opers;
}
void Channel::setNormals(std::vector<Client> normals)
{
    this->m_normals = normals;
}
void Channel::setBans(std::vector<std::string> bans)
{
    this->m_bans = bans;
}
void Channel::setTopic(std::string topic)
{
    this->m_topic = topic;
}
void Channel::setKey(std::string key)
{
    this->m_key = key;
}
void Channel::setUserCount(int count)
{
    this->m_user_count = count;
}
void Channel::setLimitCount(int count)
{
    this->m_limit_count = count;
}

void Channel::setModeInvite(bool tf)
{
    this->m_is_mode_invite = tf;
}
void Channel::setModeKey(bool tf)
{
    this->m_is_mode_key = tf;
}
void Channel::setModeTopic(bool tf)
{
    this->m_is_mode_topic = tf;
}
void Channel::setModeLimit(bool tf)
{
    this->m_is_mode_limit = tf;
}