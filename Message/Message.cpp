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
 * @brief Message 클래스의 execute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트에 대한 작업을 수행합니다.
 * m_cmds 벡터의 각 Command에 대해 반복하며, 클라이언트가 등록되어 있는지 확인한 후
 * 등록되어 있지 않다면 registerExecute 함수를 호출하고, 등록되어 있다면 commandExecute 함수를 호출합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
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

/**
 * @brief Message 클래스의 registerExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 Command 객체를 인자로 받아서
 * 해당하는 명령어를 처리하는 함수입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
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
/**
 * @brief Message 클래스의 commandExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 Command 객체를 인자로 받아서
 * 해당하는 명령을 실행하는 역할을 합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령 객체
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
