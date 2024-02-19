#include "Message.hpp"

void Message::nickExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    client.setNick(cmd->getParams()[0]);
}