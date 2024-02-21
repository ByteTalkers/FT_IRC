#include "Message.hpp"

void Message::quitExecute(Server &server, Client &client, Command *cmd)
{
    std::string reason = " :leaving";
    if (!cmd->getParams().empty())
        reason = cmd->getParams()[0];

    std::map<std::string, Channel *>::iterator it;
    for (it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
    {
        Channel *channel = it->second;
        if (channel->isMember(client))
        {
            const std::vector<Client> members = channel->getNormals();
            for (size_t i = 0; i < members.size(); ++i)
            {
                if (members[i].getNick() != client.getNick())
                {
                    // 이때 채널에 있는 모든 유저들에게 메시지를 보내야 한다.
                }
            }
            channel->partChannel(client);
        }
    }
}