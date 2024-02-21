#include "Message.hpp"

void Message::pingExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        client.addSendMsg(Response::PONG(server.getName(), ""));
    }
    else
    {
        client.addSendMsg(Response::PONG(server.getName(), cmd->getParams()[0]));
    }
    client.setWriteTypes(MYSELF);
}