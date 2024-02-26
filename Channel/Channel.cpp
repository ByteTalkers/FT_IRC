#include "Channel.hpp"

Channel::Channel(std::string name, Client *cl)
    : m_name(name), m_cretaed(time(NULL)), m_password(""), m_is_mode_invite(false), m_is_mode_key(false),
      m_is_mode_topic(false), m_is_mode_limit(false), m_is_topic_exist(false)
{
    (void)cl;
}

Channel::Channel(const Channel &src)
{
    this->m_name = src.m_name;
    this->m_cretaed = src.m_cretaed;
    this->m_operators = src.m_operators;
    this->m_normals = src.m_normals;
    this->m_bans = src.m_bans;
    this->m_invitations = src.m_invitations;
    this->m_topic = src.m_topic;
    this->m_key = src.m_key;
    this->m_limit_count = src.m_limit_count;
    this->m_is_mode_invite = src.m_is_mode_invite;
    this->m_is_mode_key = src.m_is_mode_key;
    this->m_is_mode_topic = src.m_is_mode_topic;
    this->m_is_mode_limit = src.m_is_mode_limit;

    this->m_is_topic_exist = src.m_is_topic_exist;
}

Channel::~Channel()
{
    this->m_operators.clear();
    this->m_normals.clear();
}

// 채널 나가기
void Channel::partChannel(Client &cl)
{
    std::vector<Client *>::iterator it;
    for (it = this->m_operators.begin(); it != this->m_operators.end(); ++it)
    {
        if ((*it)->getNick() == cl.getNick())
        {
            this->m_operators.erase(it);
            break;
        }
    }

    for (it = this->m_normals.begin(); it != this->m_normals.end(); ++it)
    {
        if ((*it)->getNick() == cl.getNick())
        {
            this->m_normals.erase(it);
            break;
        }
    }
}

bool Channel::checkOp(Client &cl)
{
    std::vector<Client *>::iterator it;
    for (it = this->m_operators.begin(); it != this->m_operators.end(); ++it)
    {
        if ((*it)->getNick() == cl.getNick())
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
    return this->m_normals.size();
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

bool Channel::getTopicExist() const
{
    return this->m_is_topic_exist;
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

void Channel::setTopicExist(bool tf)
{
    this->m_is_topic_exist = tf;
}

void Channel::addSendMsgAll(Server &server, const std::string &from, const std::string &cmd, const std::string &msg,
                            const std::string &last)
{
    for (std::size_t i = 0; i < m_normals.size(); i++)
    {
        if (cmd == "PRIVMSG" && m_normals[i]->getClientPrefix() == from)
        {
            continue;
        }
        m_normals[i]->addSendMsg(Response::GENERATE(from, cmd, msg + " :" + last).c_str());
        server.enableWriteEvent(m_normals[i]->getsockfd());
    }
}

void Channel::addInvitation(const std::string &user)
{
    this->m_invitations.push_back(user);
}

bool Channel::isInvited(const std::string &user) const
{
    std::deque<std::string>::const_iterator it;
    for (it = this->m_invitations.begin(); it != this->m_invitations.end(); it++)
    {
        if ((*it) == user)
        {
            return true;
        }
    }
    return false;
}

void Channel::removeInvitation(const std::string &user)
{
    std::deque<std::string>::const_iterator it;
    for (it = this->m_invitations.begin(); it != this->m_invitations.end(); it++)
    {
        if ((*it) == user)
        {
            this->m_invitations.erase(it);
            return;
        }
    }
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
            return true;
    }
    return false;
}

void Channel::addMember(Client *client)
{
    if (!isMember(*client))
        m_normals.push_back(client);
}

bool Channel::isMemberNick(std::string &nick) const
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == nick)
            return true;
    }
    return false;
}

bool Channel::isMemberNick(const std::string &nick) const
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == nick)
            return true;
    }
    return false;
}

void Channel::addOperator(const std::string &nick)
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == nick)
        {
            m_operators.push_back((*it));
            break;
        }
    }
}

void Channel::popOperator(const std::string &nick)
{
    for (std::vector<Client *>::const_iterator it = m_operators.begin(); it != m_operators.end(); ++it)
    {
        if ((*it)->getNick() == nick)
        {
            m_operators.erase(it);
            break;
        }
    }
}

bool Channel::checkOpNick(const std::string &nick) const
{
    for (std::vector<Client *>::const_iterator it = m_operators.begin(); it != m_operators.end(); ++it)
    {
        if (((*it)->getNick() == nick))
            return true;
    }
    return false;
}