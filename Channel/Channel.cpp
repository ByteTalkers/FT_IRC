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
 * @brief Channel 클래스의 partChannel 함수는 주어진 클라이언트를 채널에서 제거합니다.
 *
 * @param cl 제거할 클라이언트 객체
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
 * @brief 해당 클라이언트가 채널의 오퍼레이터인지 확인합니다.
 *
 * @param cl 확인할 클라이언트 객체
 * @return 클라이언트가 오퍼레이터인 경우 true, 그렇지 않은 경우 false를 반환합니다.
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
 * @brief 모든 클라이언트에게 메시지를 전송하는 함수입니다.
 *
 * @param server 서버 객체
 * @param from 메시지를 보내는 클라이언트의 접두사
 * @param cmd 메시지의 명령어
 * @param msg 메시지의 내용
 * @param last 메시지의 마지막 부분
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
 * @brief 채널에 초대장을 추가합니다.
 *
 * @param user 초대장을 받을 사용자의 이름
 */
void Channel::addInvitation(const std::string &user)
{
    this->m_invitations.push_back(user);
}

/**
 * @brief 해당 사용자가 초대된 사용자인지 확인합니다.
 *
 * @param user 확인할 사용자의 이름
 * @return true 사용자가 초대된 경우
 * @return false 사용자가 초대되지 않은 경우
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
 * @brief 해당 사용자의 초대장을 제거합니다.
 *
 * @param user 제거할 사용자의 이름
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
 * @brief 해당 클라이언트가 채널의 멤버인지 확인합니다.
 *
 * @param client 멤버 여부를 확인할 클라이언트 객체
 * @return 클라이언트가 채널의 멤버인 경우 true, 그렇지 않은 경우 false를 반환합니다.
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
 * @brief Channel에 멤버를 추가합니다.
 *
 * @param client 추가할 클라이언트 객체 포인터
 */
void Channel::addMember(Client *client)
{
    if (!isMember(*client))
        m_normals.push_back(client);
}

/**
 * @brief 해당 닉네임이 채널의 멤버인지 확인합니다.
 *
 * @param nick 확인할 닉네임
 * @return 멤버인 경우 true, 아닌 경우 false를 반환합니다.
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
 * @brief Channel에 운영자를 추가합니다.
 *
 * @param nick 추가할 운영자의 닉네임
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
 * @brief Channel 클래스의 popOperator 함수는 주어진 닉네임에 해당하는 오퍼레이터를 제거합니다.
 *
 * @param nick 제거할 오퍼레이터의 닉네임
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
 * @brief 해당 닉네임이 오퍼레이터 닉네임인지 확인합니다.
 *
 * @param nick 확인할 닉네임
 * @return true 오퍼레이터 닉네임인 경우
 * @return false 오퍼레이터 닉네임이 아닌 경우
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