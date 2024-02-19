#include "Message.hpp"

Message::Message(std::string &origin) : m_origin(origin)
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

// cap 명령어 처리
// void Message::handleCommandCap()
// {
//     if (m_params.empty())
//     {
//         std::cout << "m_params is empty" << std::endl;
//         return;
//     }
//     std::vector<std::string> basicCapabilities = {"multi-prefix", "sasl"};
//     std::vector<std::string> extendedCapabilities = {"multi-prefix", "sasl", "cap-notify", "batch"};
//     std::string sub_command = this->m_params[0];

//     if (sub_command == "LS") // 클라이언트가 서버에게 사용 가능한 모든 capability를 요청
//     {
//         std::string version;
//         if (this->m_params.size() > 1)
//         {
//             version = this->m_params[1];
//         }
//         if (version.empty()) // 버전 정보 없을 때 (모든 capabilities 전송)
//         {
//             std::cout << "CAP * LS :";
//             for (size_t i = 0; i < basicCapabilities.size(); i++)
//             {
//                 std::cout << basicCapabilities[i];
//                 if (i != basicCapabilities.size() - 1)
//                 {
//                     std::cout << " ";
//                 }
//             }
//             std::cout << std::endl;
//         }
//         else if (version == "302") // 특정 버전 명시
//         {
//             std::cout << "CAP * LS :";
//             for (size_t i = 0; i < extendedCapabilities.size(); i++)
//             {
//                 std::cout << extendedCapabilities[i];
//                 if (i != extendedCapabilities.size() - 1)
//                 {
//                     std::cout << " ";
//                 }
//             }
//             std::cout << std::endl;
//         }
//     }
//     return;
// }

// 커맨드 체크

void Message::execute(Server &server, Client &client)
{
    std::vector<Command*>::const_iterator it;

    for (it = this->m_cmds.begin(); it != this->m_cmds.end(); it++)
    {
        // (*it)->display();
        // 등록 여부 확인
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
    const std::string commands[13] = {"CAP", "PASS", "NICK", "USER", "PING", "QUIT", "JOIN", "PART", "MODE", "TOPIC", "INVITE", "PRIVMSG", "WHOIS"};
    
    for (int i = 0; i < 13; i++)
    {
        if (cmd == commands[i])
        {
            return i;
        }
    }
    return -1;
}

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
    client.setRegistered(true);
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
            break;
        case JOIN:
            break;
        case PART:
            break;
        case MODE:
            modeExecute(server, client, cmd);
            break;
        case TOPIC:
            break;
        case INVITE:
            break;
        case PRIVMSG:
            break;
        case WHOIS:
            whoisExecute(server, client, cmd);
            break;
        default:
            break;
    }
}

// Getter

const std::string &Message::getOrigin() const
{
    return this->m_origin;
}

const std::vector<Command*> &Message::getCmds() const
{
    return this->m_cmds;
}

// Setter

void Message::setOrigin(std::string &origin)
{
    this->m_origin = origin;
}

void Message::setCmds(std::vector<Command*> &cmds)
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


