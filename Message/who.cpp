#include "Message.hpp"

/**
 * Executes the WHO command.
 * This command retrieves information about users in a specified channel.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object.
 */
void Message::whoExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    Channel *channel = server.findChannel(cmd->getParams()[0]);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getCommand()));
        return;
    }

    client.addSendMsg(Response::RPL_WHOREPLY_352(server, client, *channel));
    client.addSendMsg(Response::RPL_ENDOFWHO_315(server, client, *channel));
}