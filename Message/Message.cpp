#include "Message.hpp"
#include "../Client/Client.hpp"

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

// 스페이스 기준으로 파싱
void Message::seperateOrigin()
{
    std::size_t start;
    std::size_t pos;

    pos = this->m_origin.find(' ');
    start = 0;
    while ((pos = this->m_origin.find(' ', start)) != std::string::npos)
    {
        std::string tmp;
        tmp = this->m_origin.substr(start, pos - start);

        // 첫 번째 문자열이면서, :로 시작하면 prefix
        if (start == 0 && tmp[0] == ':')
        {
            this->m_prefix = tmp;
        }
        else
        {
            // 커맨드가 비어있으면 커맨드 먼저 채우기
            if (this->m_command == "")
            {
                this->m_command = tmp;
            }
            else
            {
                this->m_params.push_back(tmp);
            }
        }

        start = pos + 1;
    }

    // 남은 파라미터 추가
    if (start < this->m_origin.length())
    {
        std::string last = this->m_origin.substr(start, pos - start);
        this->m_params.push_back(last);
    }
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

void Message::commandExecute(Server &server, Client &client)
{
    for (std::size_t i = 0; i < this->m_command.length(); i++)
    {
        this->m_command[i] = std::toupper(this->m_command[i]);
    }

    if (this->m_command == "CAP")
    {
    }
    else if (this->m_command == "PASS")
    {
        client.setPassword(this->m_params[0]);
    }
    else if (this->m_command == "NICK")
    {
        client.setNick(this->m_params[0]);
    }
    else if (this->m_command == "USER")
    {
        client.setUsername(this->m_params[0]);
    }
    else if (this->m_command == "PING")
    {
        cl->setSendMsg(Response::pongResponse(se.getName(), this.m_params[0]));
        // test 코드
        // std::cout << Response::pongResponse(se->getName(), this->m_params[0]) << std::endl;
    }
    else if (this->m_command == "QUIT")
    {
    }
    else if (this->m_command == "JOIN")
    {
    }
    else if (this->m_command == "PART")
    {
    }
    else if (this->m_command == "TOPIC")
    {
    }
    else if (this->m_command == "MODE")
    {
    }
    else if (this->m_command == "INVITE")
    {
    }
    else if (this->m_command == "PRIVMSG")
    {
    }
    else if (this->m_command == "WHO")
    {
    }
}

// Getter

const std::string &Message::getOrigin() const
{
    return this->m_origin;
}

const std::string &Message::getPrefix() const
{
    return this->m_prefix;
}

const std::string &Message::getCommand() const
{
    return this->m_command;
}

const std::vector<std::string> &Message::getParams() const
{
    return this->m_params;
}

// Setter

void Message::setOrigin(std::string &origin)
{
    this->m_origin = origin;
}
void Message::setPrefix(std::string &prefix)
{
    this->m_prefix = prefix;
}
void Message::setCommand(std::string &command)
{
    this->m_command = command;
}
void Message::setParams(std::vector<std::string> &params)
{
    this->m_params = params;
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
    std::cout << "prefix: " << this->getPrefix() << std::endl;
    std::cout << "command: " << this->getCommand() << std::endl;
    std::cout << "params: ";
    for (std::size_t i = 0; i < this->m_params.size(); i++)
    {
        std::cout << this->m_params[i];
        if (i != this->m_params.size() - 1)
        {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}