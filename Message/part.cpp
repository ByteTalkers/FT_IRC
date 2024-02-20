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

    // 1. 파라미터가 없을 경우
    if (cmd->getParams().empty())
    {
        // 채널에 속해 있지 않을 경우
        if (client.getCurChannel().empty())
        {
            client.addSendMsg("You're not on that channel");
            return;
        }
        else // 채널에 속해 있는 경우
            channels.push_back(client.getCurChannel());
    }
    else
        parseChannelAndReason(cmd->getParams(), channels, reason);

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
        std::map<std::string, Channel *>::iterator it = server.getChannels().find(channelName);
        if (it == server.getChannels().end()) // 채널이 존재하지 않는 경우
        {
            client.addSendMsg(channelName + " :No such channel");
            continue;
        }

        Channel *channel = it->second;
        if (!channel) // 존재하지 않는 채널
        {
            client.addSendMsg(channelName + " :No such channel");
            continue;
        }

        if (!channel->isMember(client)) // 채널에 속해 있지 않은 경우
        {
            client.addSendMsg(channelName + " You're not on that channel");
            continue;
        }

        channel->partChannel(client); // 채널 나가기
        std::string message = client.getNick() + " has left " + channelName;
        if (!reason.empty())
            message += " [" + reason + "]";

        for (std::vector<Client>::iterator it = channel->getNormals().begin(); it != channel->getNormals().end(); ++it)
            it->addSendMsg(message);
    }
}