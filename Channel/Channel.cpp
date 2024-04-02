#include "Channel.hpp"

Channel::Channel(std::string name)
    : m_name(name), m_cretaed(time(NULL)), m_password(""), m_is_mode_invite(false), m_is_mode_key(false),
      m_is_mode_topic(false), m_is_mode_limit(false), m_is_topic_exist(false)
{
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

/**
 * @brief Removes a client from the channel.
 *
 * This function removes the specified client from the channel. The client will no longer be a member of the channel.
 *
 * @param cl The client to be removed from the channel.
 */
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

/**
 * Checks if a client is an operator in the channel.
 *
 * @param cl The client to check.
 * @return True if the client is an operator, false otherwise.
 */
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

/**
 * Sends a message to all users in the channel.
 *
 * @param server The server object.
 * @param from The sender of the message.
 * @param cmd The command associated with the message.
 * @param msg The content of the message.
 * @param last The last message in the conversation.
 */
void Channel::addSendMsgAll(Server &server, const std::string &from, const std::string &cmd, const std::string &msg,
                            const std::string &last)
{
    for (std::size_t i = 0; i < m_normals.size(); i++)
    {
        if ((cmd == "PRIVMSG" || cmd == "QUIT") && m_normals[i]->getClientPrefix() == from)
        {
            continue;
        }
        m_normals[i]->addSendMsg(Response::GENERATE(from, cmd, msg + " :" + last).c_str());
        server.enableWriteEvent(m_normals[i]->getsockfd());
    }
}

/**
 * @brief Adds an invitation for a user to join the channel.
 *
 * This function adds an invitation for the specified user to join the channel.
 *
 * @param user The username of the user to invite.
 */
void Channel::addInvitation(const std::string &user)
{
    this->m_invitations.push_back(user);
}

/**
 * Checks if a user is invited to the channel.
 *
 * @param user The username of the user to check.
 * @return True if the user is invited, false otherwise.
 */
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

/**
 * @brief Removes an invitation for a user from the channel.
 *
 * This function removes the specified user from the list of invitations in the channel.
 * If the user is found in the list, they will be removed and the function will return.
 * If the user is not found, the function will have no effect.
 *
 * @param user The username of the user to remove from the invitations.
 */
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

/**
 * Checks if the given client is a member of the channel.
 *
 * @param client The client to check.
 * @return True if the client is a member of the channel, false otherwise.
 */
bool Channel::isMember(Client &client) const
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == client.getNick())
            return true;
    }
    return false;
}

/**
 * Adds a member to the channel.
 *
 * @param client A pointer to the client to be added as a member.
 */
void Channel::addMember(Client *client)
{
    if (!isMember(*client))
        m_normals.push_back(client);
}

/**
 * Checks if a given nickname is a member of the channel.
 *
 * @param nick The nickname to check.
 * @return True if the nickname is a member of the channel, false otherwise.
 */
bool Channel::isMemberNick(std::string &nick) const
{
    for (std::vector<Client *>::const_iterator it = m_normals.begin(); it != m_normals.end(); ++it)
    {
        if ((*it)->getNick() == nick)
            return true;
    }
    return false;
}

/**
 * @brief Adds an operator to the channel.
 *
 * This function adds a client with the specified nickname as an operator to the channel.
 * The client must already be a member of the channel's normal clients list.
 *
 * @param nick The nickname of the client to be added as an operator.
 */
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

/**
 * @brief Removes the operator with the specified nickname from the channel.
 *
 * This function searches for an operator with the given nickname in the list of operators
 * and removes it from the channel if found. If multiple operators have the same nickname,
 * only the first occurrence will be removed.
 *
 * @param nick The nickname of the operator to be removed.
 */
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

/**
 * Checks if a given nickname is an operator in the channel.
 *
 * @param nick The nickname to check.
 * @return True if the nickname is an operator, false otherwise.
 */
bool Channel::checkOpNick(const std::string &nick) const
{
    for (std::vector<Client *>::const_iterator it = m_operators.begin(); it != m_operators.end(); ++it)
    {
        if (((*it)->getNick() == nick))
            return true;
    }
    return false;
}