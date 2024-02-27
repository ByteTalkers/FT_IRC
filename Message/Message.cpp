#include "Message.hpp"
Message::Message(const std::string &origin) : m_origin(origin)
{
}

Message::~Message()
{
}

Message::Message(const Message &src)
{
    *this = src;
}

Message &Message::operator=(Message const &rhs)
{
    if (this != &rhs)
    {
        this->m_origin = rhs.m_origin;
        this->m_cmds = rhs.m_cmds;
    }
    return *this;
}

/**
 * 문자열 끝에 crlf가 있는지 확인하는 함수
 */
bool Message::crlfCheck()
{
    std::string crlf;

    crlf = this->m_origin.substr(this->m_origin.length() - 2);
    if (crlf == "\r\n")
    {
        this->m_origin = this->m_origin.substr(0, this->m_origin.length() - 2);
        return true;
    }
    return false;
}

/**
 * 커맨드 실행부
 * 1. 등록 전 실행부 => registerExecute
 * 2. 등록 후 실행부 => commandExecute
 */
void Message::execute(Server &server, Client &client)
{
    std::vector<Command>::iterator it;

    for (it = this->m_cmds.begin(); it != this->m_cmds.end(); it++)
    {
        if (!client.getRegisterd())
        {
            registerExecute(server, client, &(*it));
        }
        else
        {
            commandExecute(server, client, &(*it));
        }
    }
}

/**
 * 커맨드를 찾는 함수
 */
static int findCommands(const std::string &cmd)
{
    const std::string commands[COMMAND_SIZE] = {"CAP",  "PASS", "NICK",  "USER",   "PING",    "QUIT", "JOIN",
                                                "PART", "MODE", "TOPIC", "INVITE", "PRIVMSG", "WHO",  "KICK"};

    for (int i = 0; i < COMMAND_SIZE; i++)
    {
        if (cmd == commands[i])
        {
            return i;
        }
    }
    return -1;
}

/**
 * 등록 전 실행되는 커맨드 함수
 * CAP, PASS, NICK, USER 명령만 처리
 */
void Message::registerExecute(Server &server, Client &client, Command *cmd)
{
    int cmd_num = findCommands(cmd->getCommand());

    switch (cmd_num)
    {
    case CAP:
        break;
    case PASS:
        registerPassExecute(server, client, cmd);
        break;
    case NICK:
        registerNickExecute(server, client, cmd);
        break;
    case USER:
        registerUserExecute(server, client, cmd);
        break;
    }

    if (client.getIsRegisterFlags()[PASS_REG] && client.getIsRegisterFlags()[USER_REG] &&
        client.getIsRegisterFlags()[NICK_REG])
    {
        client.setRegistered(true);
        client.addSendMsg(Response::RPL_WELCOME_001(server, client));
    }
    else
    {
        client.setRegistered(false);
    }

    if (!client.getSendMsg().empty())
    {
        client.setWriteTypes(MYSELF);
    }
}

/**
 * 등록 후 실행되는 커맨드 함수
 * CAP, PASS, NICK, USER, PING, QUIT, JOIN, PART, MODE, TOPIC, INVITE, PRIVMSG, WHO, KICK 처리
 */
void Message::commandExecute(Server &server, Client &client, Command *cmd)
{
    int cmd_num = findCommands(cmd->getCommand());

    switch (cmd_num)
    {
    case CAP:
        break;
    case PASS:
        passExecute(server, client, cmd);
        break;
    case NICK:
        nickExecute(server, client, cmd);
        break;
    case USER:
        userExecute(server, client, cmd);
        break;
    case PING:
        pingExecute(server, client, cmd);
        break;
    case QUIT:
        quitExecute(client, cmd);
        break;
    case JOIN:
        joinExecute(server, client, cmd);
        break;
    case PART:
        partExecute(server, client, cmd);
        break;
    case MODE:
        modeExecute(server, client, cmd);
        break;
    case TOPIC:
        topicExecute(server, client, cmd);
        break;
    case INVITE:
        inviteExecute(server, client, cmd);
        break;
    case PRIVMSG:
        privmsgExecute(server, client, cmd);
        break;
    case WHO:
        whoExecute(server, client, cmd);
        break;
    case KICK:
        kickExecute(server, client, cmd);
        break;
    default:
        break;
    }

    if (!client.getSendMsg().empty())
        client.setWriteTypes(MYSELF);
    // 만약 irc 메소드가 KICK, QUIT, PART면 채널을 지울 수도 있음.
    if (cmd_num == QUIT || cmd_num == KICK || cmd_num == PART)
        server.delEmptyChannel();
}

/* getter */
const std::string &Message::getOrigin() const
{
    return this->m_origin;
}

const std::vector<Command> &Message::getCmds() const
{
    return this->m_cmds;
}

/* setter */
void Message::setOrigin(std::string &origin)
{
    this->m_origin = origin;
}

void Message::setCmds(std::vector<Command> &cmds)
{
    this->m_cmds = cmds;
}

/**
 * Message 멤버변수 보여주는 함수
 */
// void Message::display()
// {
//     std::cout << "origin: " << this->getOrigin();
//     if (!this->crlfCheck())
//     {
//         std::cout << std::endl << "Invalid" << std::endl;
//         return;
//     }
//     std::cout << std::endl;
// }
