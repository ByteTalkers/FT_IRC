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
            client.addSendMsg("You're not on that channel");
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
            // 403: No such channel
            continue;
        }

        Channel *channel = it->second;
        if (!channel->isMember(client))
        {
            // 442: You're not on that channel
            continue;
        }

        channel->partChannel(client);
        std::string message = client.getNick() + " has left " + channelName;
        if (!reason.empty())
            message += " [" + reason + "]";

        std::vector<Client> allMembers = channel->getNormals();
        std::vector<Client>::iterator it_member;
        for (it_member = allMembers.begin(); it_member != allMembers.end(); ++it_member)
        {
            Client &member = *it_member;
            if (member.getNick() != client.getNick())
                member.addSendMsg(message);
        }
    }
}