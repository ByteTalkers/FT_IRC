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

void Message::handleCommandJoin(Server &server, Client &client)
{
    if (this->m_params.empty())
    {
        // 채널 이름이 없는 경우 에러 메시지 전송
        return;
    }

    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    // 채널 이름과 키를 파싱하는 로직
    std::istringstream iss(this->m_params[0]);
    std::string token;
    while (std::getline(iss, token, ','))
    {
        // 채널 이름 유효성 검사 로직 추가
        channelNames.push_back(token);
    }

    // 키 파싱
    if (this->m_params.size() > 1)
    {
        std::istringstream keyIss(this->m_params[1]);
        while (std::getline(keyIss, token, ','))
        {
            keys.push_back(token);
        }
    }

    // 채널 처리 로직
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        // 채널 이름 유효성 검사
        if (channelNames[i][0] != '#' && channelNames[i][0] != '&')
        {
            client.setSendMsg("ERROR :Invalid channel name " + channelNames[i]);
            continue;
        }

        std::map<std::string, Channel *> &channelsMap = server.getChannels();
        std::map<std::string, Channel *>::iterator it = channelsMap.find(channelNames[i]);
        std::string key;

        if (i < keys.size())
            key = keys[i];
        else
            key = "";

        if (it == channelsMap.end())
        {
            // 채널이 존재하지 않으면 새로 생성
            Channel *newChannel = new Channel(channelNames[i], client); // 적절한 생성자 가정
            if (!key.empty())
                newChannel->setKey(key); // 채널 키 설정
            channelsMap[channelNames[i]] = newChannel;
            // 클라이언트에게 채널 참여 메시지 전송 등의 추가 로직
        }
        else
        {
            Channel *channel = it->second;
            if (channel->isInvited(client.getNick()) || channel->checkKey(key))
            {
                if (!channel->isMember(client))
                {
                    channel->addMember(client);
                    // 클라이언트에게 채널 참여 메시지 전송 등의 추가 로직
                }
                else
                {
                    client.setSendMsg("ERROR :You're already a member of " + channelNames[i]);
                }
            }
            else
            {
                client.setSendMsg("ERROR :Invalid key or not invited to " + channelNames[i]);
            }
        }
    }

    // JOIN 성공 시 응답 메시지 전송 로직
}

void Message::commandExecute(Server &server, Client &client)
{
    std::vector<Command *>::const_iterator it;

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
        client.setSendMsg(Response::pongResponse(server.getName(), this->m_params[0]));
        // test 코드
        // std::cout << Response::pongResponse(se->getName(), this->m_params[0]) << std::endl;
    }
    else if (this->m_command == "QUIT")
    {
        std::string quitMsg;

        quitMsg = this->m_params[0];
        // 조건 1 quit 뒤에 parameter가 없을 때
        if (quitMsg.empty())
        {
            // 클라이언트가 현재 참여하고 있는 모든 채널에서
        }
        // 조건 2 quit 뒤에 parameter가 있을 때
        else
        {
        }
    }
    else if (this->m_command == "JOIN")
    {
        handleCommandJoin(server, client);
    }
    else if (this->m_command == "PART") // 채널 나가기
    {
        // 채널에 없는데 나가려 할때 처리하기
        // 여러 채널에 참여하고 있는데 PART만 눌렀을 때 해당 채널에서 나가지고, 채널 목록 돌면서 채널에 속해
        // 있으면 첫번째 채널 목록으로 들어가기
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
    const std::string commands[13] = {"CAP",  "PASS", "NICK",  "USER",   "PING",    "QUIT", "JOIN",
                                      "PART", "MODE", "TOPIC", "INVITE", "PRIVMSG", "WHOIS"};

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
