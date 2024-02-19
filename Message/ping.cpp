#include "Message.hpp"

void pingExecute(Server &server, Client &client, Command *cmd)
{
    client.addSendMsg(Response::pongResponse(server.getName(), cmd->getParams()[0]));
}