#include "Message.hpp"

void Message::passExecute(Server &server, Client &client, Command *cmd)
{
    (void)client;
    if (server.getPassword() != cmd->getParams()[0])
    {
        //err
    }
}