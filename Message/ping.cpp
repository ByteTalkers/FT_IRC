#include "Message.hpp"

/**
 * Executes the PING command.
 * If the command has no parameters, sends a PONG response to the client with an empty parameter.
 * If the command has a parameter, sends a PONG response to the client with the same parameter.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The PING command object.
 */
void Message::pingExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        client.addSendMsg(Response::PONG(server, ""));
    }
    else
    {
        client.addSendMsg(Response::PONG(server, cmd->getParams()[0]));
    }
}