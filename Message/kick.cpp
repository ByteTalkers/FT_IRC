#include "Message.hpp"

void Message::kickExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    std::string channelName = cmd->getParams()[0];
    std::string targetNick = cmd->getParams()[1];
    std::string reason = cmd->getParamsCount() > 2 ? cmd->getParams()[2] : "";

    Channel *channel = server.findChannel(channelName);
    if (!channel)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channelName));
        return;
    }

    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }

    Client *targetClient = server.findClient(targetNick);
    if (!targetClient || !channel->isMember(*targetClient))
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, targetNick));
        return;
    }

    channel->partChannel(*targetClient);
    std::string kickMessage = "KICK " + channelName + " " + targetNick + " :" + reason;
    channel->addSendMsgAll(server, client.getNick(), "KICK", kickMessage);
}