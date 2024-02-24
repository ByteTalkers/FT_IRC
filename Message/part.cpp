#include "Message.hpp"

static void parseChannelAndReason(const std::vector<std::string> &params, std::vector<std::string> &channels,
                                  std::string &reason)
{
    std::istringstream iss(params[0]);
    std::string channelName;
    while (std::getline(iss, channelName, ','))
    {
        channels.push_back(channelName);
    }
    // 두 번째 파라미터부터는 이유로 처리
    for (size_t i = 1; i < params.size(); ++i)
    {
        if (i > 1)
            reason += " ";
        reason += params[i];
    }
}

void Message::partExecute(Server &server, Client &client, Command *cmd)
{
    std::vector<std::string> channels;
    std::string reason;

    cmd->display();
    if (cmd->getParams().empty()) // /part     /part #channel
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, "PART"));
    }
    parseChannelAndReason(cmd->getParams(), channels, reason);

    std::vector<std::string>::iterator it_channel;
    for (it_channel = channels.begin(); it_channel != channels.end(); ++it_channel)
    {
        std::string channelName = *it_channel;
        Channel *channel = server.findChannel(channelName);

        if (channel == NULL)
        {
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channelName));
            continue;
        }
        if (!channel->isMember(client))
        {
            client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
            continue;
        }
        if (!reason.empty())
            channel->addSendMsgAll(server, client.getNick(), "PART", channelName, reason);
        else
            channel->addSendMsgAll(server, client.getNick(), "PART", "", channelName);
        channel->partChannel(client);
    }
}