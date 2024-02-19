#include "Message.hpp"

void Message::userExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    client.setUsername(cmd->getParams()[0]);
    client.addSendMsg(Response::rplWelcome_001(server.getName(), client.getNick(), client.getUser()));
    std::cout << "this is user" << std::endl;
}