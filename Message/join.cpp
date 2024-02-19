#include "Message.hpp"

void Message::joinExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        // 채널 이름이 없는 경우 에러 메시지 전송(Irssi: Not enough parameters given)
        client.addSendMsg("Irssi: Not enough parameters given");
        return;
    }

    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    // 채널 이름과 키를 파싱하는 로직
    std::istringstream iss(cmd->getParams()[0]);
    std::string token;
    while (std::getline(iss, token, ','))
    {
        // 채널 이름 유효성 검사 로직 추가
        channelNames.push_back(token);
    }

    // 키 파싱
    if (cmd->getParams().size() > 1)
    {
        const std::string &keyParam = cmd->getParams()[1];
        size_t pos = 0;
        size_t keyStart = 0;

        for (size_t i = 0; i < channelNames.size(); ++i)
        {
            if ((pos = keyParam.find(',', keyStart)) != std::string::npos)
            {
                keys.push_back(keyParam.substr(keyStart, pos - keyStart));
                keyStart = pos + 1;
            }
            else if (i == 0)
            {
                keys.push_back(keyParam.substr(keyStart));
                break;
            }
            else
            {
                keys.push_back("");
            }
        }
    }

    // 채널 처리 로직
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        // 채널 이름 유효성 검사
        if (channelNames[i][0] != '#' && channelNames[i][0] != '&')
        {
            client.addSendMsg("ERROR :Invalid channel name " + channelNames[i]);
            continue;
        }

        std::string channelName = channelNames[i];
        std::string key = i < keys.size() ? keys[i] : "";

        Channel *channel = NULL;
        if (server.getChannels().find(channelName) == server.getChannels().end())
        {
            channel = new Channel(channelName, client);
            server.getChannels()[channelName] = channel;
            if (!key.empty())
                channel->setKey(key);
        }
        else
        {
            channel = server.getChannels()[channelName];
        }

        if (channel->isInvited(client.getNick()) || channel->checkKey(key))
        {
            if (!channel->isMember(client))
            {
                channel->addMember(client);

                // 기존 클라이언트들에게 새 클라이언트의 입장 알림
                std::vector<Client> members = channel->getNormals();
                for (std::vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
                {
                    if (it->getNick() != client.getNick())
                    {
                        // nana_ [codespace@127.0.0.1] has joined #a -> 이런 형식으로 출력
                        client.addSendMsg(client.getNick() + " has joined " + channelName);
                    }
                }

                // 새 클라이언트에게 채널 정보 전송
                // Irssi: #a: Total of 2 nicks [1 ops, 0 halfops, 0 voices, 1 normal]
                // Channel #a created Mon Feb 19 12:05:15 2024
                // Irssi: Join to #a was synced in 0 secs
            }
            else
            {
                // 이미 채널에 있는 경우 에러 메시지 전송
                client.addSendMsg("ERROR :You are already in " + channelName);
            }
        }
        else
        {
            client.addSendMsg("ERROR :Invalid key or not invited to " + channelName);
        }
    }
    // JOIN 성공 시 응답 메시지 전송 로직
}