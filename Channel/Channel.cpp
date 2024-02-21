#include "Channel.hpp"

Channel::Channel(std::string name, Client *cl) : m_name(name), m_cretaed(time(NULL)), m_password(""), m_user_count(1)
{
    this->m_operators.push_back(cl);
}

Channel::Channel(const Channel &src)
{
    this->m_name = src.m_name;				
    this->m_cretaed = src.m_cretaed;
    this->m_operators = src.m_operators;
    this->m_normals = src.  m_normals;
    this->m_bans = src.m_bans;
    this->m_invitations = src.m_invitations;
    this->m_topic = src.m_topic;
    this->m_key = src.m_key;
    this->m_user_count = src.m_user_count;
    this->m_limit_count = src.m_limit_count;

    this->m_is_mode_invite = src.m_is_mode_invite;
    this->m_is_mode_key = src.m_is_mode_key;
    this->m_is_mode_topic = src.m_is_mode_topic;
    this->m_is_mode_limit = src.m_is_mode_limit;

    this->m_is_set_topic = src.m_is_set_topic;
}

Channel::~Channel()
{
    this->m_operators.clear();
    this->m_normals.clear();
}

// 채널에 들어옴
void Channel::joinChannel(Client *cl)
{
    if (this->m_is_mode_invite)
    {
        // todo: 초대 여부 확인
    }
    if (this->m_is_mode_key)
    {
        // todo: 비밀번호 입력 확인
    }
    if (this->m_is_mode_limit)
    {
        if (this->m_user_count == this->m_limit_count)
        {
            // todo: error reply 설정
        }
    }

    this->m_user_count++;
    cl->setCurChannel(this->getName()); // 클라이언트의 현재 채널이름 설정
    this->m_normals.push_back(cl);     // 목록에 넣기
}

// 채널 나가기
void Channel::partChannel(Client *cl)
{
    this->m_user_count--;
    for (std::size_t i = 0; i < this->m_operators.size(); i++)
    {
        if (this->m_operators[i]->getNick() == cl->getNick())
        {
            this->m_operators.erase(this->m_operators.begin() + i);
            return;
        }
    }
    for (std::size_t i = 0; i < this->m_normals.size(); i++)
    {
        if (this->m_normals[i]->getNick() == cl->getNick())
        {
            cl->setCurChannel(NULL); // 클라이언트의 현재 채널이름도 NULL로
            this->m_normals.erase(this->m_normals.begin() + i);
            return;
        }
    }
}

bool Channel::checkOp(Client cl)
{
    std::vector<Client *>::iterator it;
    for (it = this->m_operators.begin(); it < this->m_operators.end(); it++)
    {
        if ((*it) == &cl) 
        {
            return true;
        }
    }
    return false;
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

std::vector<Client *> Channel::getOperators() const
{
    return this->m_operators;
}

std::vector<Client *> Channel::getNormals() const
{
    return this->m_normals;
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

bool Channel::getSetTopic() const
{
    return this->m_is_set_topic;
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

void Channel::setOperators(std::vector<Client *> opers)
{
    this->m_operators = opers;
}

void Channel::setNormals(std::vector<Client *> normals)
{
    this->m_normals = normals;
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

void Channel::setSetTopic(bool tf)
{
    this->m_is_set_topic = tf;
}

void Channel::addSendMsgAll(Server &server, const std::string &from, const std::string &cmd, const std::string &msg)
{
    std::vector<Client *>::iterator it;
    for (it = this->m_normals.begin(); it != this->m_normals.end(); it++)
    {
        (*it)->addSendMsg(Response::GENERATE(from, cmd, this->m_name + " :" + msg).c_str());
        server.enableWriteEvent((*it)->getsockfd());
    }
}

void Channel::addInvitation(const std::string &user)
{
    this->m_invitations[user] = true;
}

bool Channel::isInvited(const std::string &user) const
{
    return m_invitations.find(user) != m_invitations.end();
}

void Channel::removeInvitation(const std::string &user)
{
    this->m_invitations.erase(user);
}

bool Channel::checkKey(const std::string &key)
{
    return this->m_key == key;
}

bool Channel::checkPassword(const std::string &password)
{
    return this->m_password == password;
}

bool Channel::isMember(Client &client) const
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == client.getNick())
        {
            return true;
        }
    }
    return false;
}

void Channel::addMember(Client *client)
{
    if (!isMember(*client))
    {
        m_normals.push_back(client);
    }
}
