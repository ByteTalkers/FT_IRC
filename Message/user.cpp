#include "Message.hpp"

void Message::userExecute(Server &server, Client &client)
{
    (void)server;
    client.setUsername(this->m_params[0]);
    client.addSendMsg(Response::rplWelcome_001(server.getName(), client.getNick(), client.getUser()));
    std::cout << "this is user" << std::endl;
}