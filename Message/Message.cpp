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
 * Executes the commands in the message for the given server and client.
 *
 * This function iterates through the commands in the message and executes them
 * based on the client's registration status. If the client is not registered,
 * the registerExecute function is called for each command. Otherwise, the
 * commandExecute function is called for each command.
 *
 * @param server The server object.
 * @param client The client object.
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
 * @brief Finds commands in a given string.
 *
 * This function searches for commands within the provided string and returns the number of commands found.
 *
 * @param cmd The string to search for commands.
 * @return The number of commands found in the string.
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

/**
 * Executes the registration process for a client based on the received command.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object.
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

/*
등록 후 실행되는 커맨드 함수
CAP, PASS, NICK, USER, PING, QUIT, JOIN, PART, MODE, TOPIC, INVITE, PRIVMSG, WHO, KICK 처리
*/

/**
 * Executes the command received from a client.
 *
 * This function takes a reference to a `Server` object, a `Client` object, and a `Command` object as parameters.
 * It determines the type of command and calls the corresponding execution function.
 *
 * @param server The reference to the `Server` object.
 * @param client The reference to the `Client` object.
 * @param cmd The pointer to the `Command` object representing the received command.
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
