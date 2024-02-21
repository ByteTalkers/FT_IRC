#include "Message.hpp"

void Message::userExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    client.setUsername(cmd->getParams()[0]);
    client.addSendMsg(Response::RPL_WELCOME_001(server.getName(), client.getNick(), client.getUser()));
    client.setWriteTypes(MYSELF);
    std::cout << "this is user" << std::endl;
}