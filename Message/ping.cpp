#include "Message.hpp"

void Message::pingExecute(Server &server, Client &client, Command *cmd)
{
    client.addSendMsg(Response::pongResponse(server.getName(), cmd->getParams()[0]));
}