#include "Message.hpp"

static void parseChannelParams(const std::string &param, std::vector<std::string> &channelNames)
{
    std::istringstream iss(param);
    std::string channelToken;
    while (std::getline(iss, channelToken, ','))
    {
        channelNames.push_back(channelToken);
    }
}

static void parseKeyParams(const std::string &param, std::vector<std::string> &keys)
{
    std::istringstream keyIss(param);
    std::string keyToken;
    while (std::getline(keyIss, keyToken, ','))
    {
        keys.push_back(keyToken);
    }
}

static void checkInviteMode(Server &server, Client &client, Channel &channel, std::string &key)
{
    if (!channel.isInvited(client.getNick()))
    {
        // 키 체크
        if (!channel.checkKey(key))
        {
            client.addSendMsg(Response::ERR_BADCHANNELKEY_475(server, client, channel));
        }
        // 인원 수 체크
        if (channel.getModeLimit() && channel.getLimitCount() <= channel.getUserCount())
        {
            client.addSendMsg(Response::ERR_CHANNEL_IS_FULL_471(server, client, channel));
        }
        client.addSendMsg(Response::ERR_INVITEONLYCHAN_473(server, client, channel));
    }
    else
    {
        // 채널 초대 목록에서 제거
        channel.removeInvitation(client.getNick());
        // 채널에 멤버 추가
        channel.addMember(&client);
    }
}

void Message::joinExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        // 채널 이름이 없는 경우 에러 메시지 전송(Irssi: Not enough parameters given)
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    parseChannelParams(cmd->getParams()[0], channelNames);
    if (cmd->getParams().size() > 1)
        parseKeyParams(cmd->getParams()[1], keys);

    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        std::string channelName = channelNames[i];
        std::string key = i < keys.size() ? keys[i] : "";

        Channel *channel = server.findChannel(channelName);
        bool isNewChannel = false;
        if (channel == NULL)
        {
            channel = new Channel(channelName, &client);
            server.addChannel(channelName, channel);
            isNewChannel = true;
        }

        if (channel->isMember(client))
        {
            // 에러메시지 잘 모르겠음 (irc.example.com 443 nana nana_ #a :is already on channel) -> nana 와 nana_
            // 구분안됨
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, *channel));
            client.addSendMsg(Response::RPL_NAMREPLY_353(server, client, *channel));
            client.addSendMsg(Response::RPL_ENDOFNAMES_366(server, client, *channel));
            /*이렇게 하면 사용자가 이미 속해 있는 채널에 다시 JOIN 명령을 시도했을 때, 채널의 현재 상태 정보를 다시
             * 제공하여 사용자가 이미 채널에 있다는 사실을 간접적으로 알릴 수 있습니다.*/
            return;
        }
        if (channel->getModeInvite()) // 초대 모드
        {
            checkInviteMode(server, client, *channel, key);
        }
        // 키 체크
        if (channel->getModeKey() && !channel->checkKey(key))
        {
            client.addSendMsg(Response::ERR_BADCHANNELKEY_475(server, client, *channel));
            continue;
        }
        // 채널 인원 수 체크
        if (channel->getModeLimit() && channel->getLimitCount() <= channel->getUserCount())
        {
            client.addSendMsg(Response::ERR_CHANNEL_IS_FULL_471(server, client, *channel));
            continue;
        }

        channel->addMember(&client);
        if (isNewChannel)
        {
            channel->addOperator(client.getNick());
        }
        client.addSendMsg(Response::GENERATE(client.getNick(), "JOIN", " :" + channelName));
        client.addSendMsg(Response::RPL_NAMREPLY_353(server, client, *channel));
        client.addSendMsg(Response::RPL_ENDOFNAMES_366(server, client, *channel));
    }
}
