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

        for (size_t i = 0; i < channelNames.size(); +i)
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
                    client.addSendMsg(":" + client.getNick() + " JOIN " + channelNames[i]);
                }
                else
                {
                    client.addSendMsg("ERROR :You're already a member of " + channelNames[i]);
                }
            }
            else
            {
                client.addSendMsg("ERROR :Invalid key or not invited to " + channelNames[i]);
            }
        }
    }

    // JOIN 성공 시 응답 메시지 전송 로직
}