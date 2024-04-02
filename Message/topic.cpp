#include "Message.hpp"

/**
 * Executes the TOPIC command for a given server, client, and command.
 *
 * This function is responsible for handling the TOPIC command, which allows a client to
 * set or retrieve the topic of a channel. It checks the parameters provided by the client,
 * finds the specified channel, and performs the necessary checks and actions based on the
 * channel's properties and the client's permissions.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object representing the TOPIC command.
 */
void Message::topicExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    std::string channel_name = cmd->getParams()[0];
    Channel *channel = server.findChannel(channel_name);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getParams()[0]));
        return;
    }

    if (cmd->getParamsCount() == 1)
    {
        if (channel->getTopicExist())
        {
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, *channel));
            return;
        }
        client.addSendMsg(Response::RPL_NOTOPIC_331(server, client, *channel));
        return;
    }

    if (!channel->isMember(client))
    {
        client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
        return;
    }

    if (channel->getModeTopic() && !channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if ((channel->getModeTopic() && channel->checkOp(client)) || !channel->getModeTopic())
    {
        channel->setTopicExist(true);
        channel->setTopic(cmd->getParams()[1]);
        channel->addSendMsgAll(server, client.getClientPrefix(), "TOPIC", channel_name, cmd->getParams()[1]);
    }
}
