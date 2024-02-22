#include "Message.hpp"

Message::Message()
{
}

Message::Message(const std::string &origin) : m_origin(origin)
{
}

Message::~Message()
{
}

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
    std::vector<Command *>::const_iterator it;

    for (it = this->m_cmds.begin(); it != this->m_cmds.end(); it++)
    {
        if (!client.getRegisterd())
        {
            registerExecute(server, client, *it);
        }
        else
        {
            commandExecute(server, client, *it);
        }
    }
}

int findCommands(const std::string &cmd)
{
    const std::string commands[14] = {"CAP",  "PASS", "NICK",  "USER",   "PING",    "QUIT",  "JOIN",
                                      "PART", "MODE", "TOPIC", "INVITE", "PRIVMSG", "WHOIS", "KICK"};

    for (int i = 0; i < 13; i++)
    {
        if (cmd == commands[i])
        {
            return i;
        }
    }
    return -1;
}

/**
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
        passExecute(server, client, cmd);
        break;
    case NICK:
        nickExecute(server, client, cmd);
        break;
    case USER:
        userExecute(server, client, cmd);
        break;
    }

    if (client.getIsRegisterFlags()[PASS_REG] && client.getIsRegisterFlags()[USER_REG] &&
        client.getIsRegisterFlags()[NICK_REG])
    {
        client.setRegistered(true);
    }
    else
    {
        client.setRegistered(false);
    }
}

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
        quitExecute(server, client, cmd);
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
        break;
    case PRIVMSG:
        privmsgExecute(server, client, cmd);
        break;
    case WHOIS:
        whoisExecute(server, client, cmd);
        break;
    case KICK:
        kickExecute(server, client, cmd);
        break;
    default:
        break;
    }
    client.setWriteTypes(MYSELF);
}

// Getter
const std::string &Message::getOrigin() const
{
    return this->m_origin;
}

const std::vector<Command *> &Message::getCmds() const
{
    return this->m_cmds;
}

// Setter
void Message::setOrigin(std::string &origin)
{
    this->m_origin = origin;
}

void Message::setCmds(std::vector<Command *> &cmds)
{
    this->m_cmds = cmds;
}

// test
void Message::display()
{
    std::cout << "origin: " << this->getOrigin();
    if (!this->crlfCheck())
    {
        std::cout << std::endl << "Invalid" << std::endl;
        return;
    }
    std::cout << std::endl;
}
