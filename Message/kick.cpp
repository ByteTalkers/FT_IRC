#include "Message.hpp"

/**
 * Executes the KICK command, which allows a client with operator privileges to kick a user from a channel.
 *
 * @param server The server object.
 * @param client The client object representing the client who issued the command.
 * @param cmd The command object containing the command parameters.
 */
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
    if (!targetClient)
    {
        client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
        return;
    }
    if (!channel->isMember(*targetClient))
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, targetNick));
        return;
    }

    channel->addSendMsgAll(server, client.getClientPrefix(), "KICK", channelName + " " + targetNick, reason);
    channel->partChannel(*targetClient);
}