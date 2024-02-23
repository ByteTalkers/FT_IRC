#include "Message.hpp"

static void parseChannelAndReason(const std::vector<std::string> &params, std::vector<std::string> &channels,
                                  std::string &reason)
{
    if (!params.empty())
    {
        std::istringstream iss(params[0]);
        std::string channelName;
        while (std::getline(iss, channelName, ','))
        {
            channelName.erase(std::remove(channelName.begin(), channelName.end(), ' '), channelName.end());
            if (!channelName.empty())
                channels.push_back(channelName);
        }
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

    if (cmd->getParams().empty())
    {
        if (client.getCurChannel().empty())
        {
            std::string curChannel = client.getCurChannel();
            client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *server.getChannels()[curChannel]));
            return;
        }
        else
            channels.push_back(client.getCurChannel());
    }
    else
        parseChannelAndReason(cmd->getParams(), channels, reason);

    std::vector<std::string>::iterator it_channel;
    for (it_channel = channels.begin(); it_channel != channels.end(); ++it_channel)
    {
        std::string channelName = *it_channel;
        std::map<std::string, Channel *>::iterator it = server.getChannels().find(channelName);
        if (it == server.getChannels().end())
        {
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channelName));
            continue;
        }

        Channel *channel = it->second;
        if (!channel->isMember(client))
        {
            client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
            continue;
        }
        channel->partChannel(client);
        // if (channel->getUserCount() == 0)
        // {
        //     server.getChannels().erase(it);
        //     // delete channel;
        // }
        if (!reason.empty())
            channel->addSendMsgAll(server, client.getNick(), "PART " + channelName, reason);
        else
            channel->addSendMsgAll(server, client.getNick(), "PART", channelName);
    }
}
