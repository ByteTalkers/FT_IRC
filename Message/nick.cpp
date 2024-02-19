#include "Message.hpp"

void Message::nickExecute(Server &server, Client &client)
{
    (void)server;
    client.setNick(this->m_params[0]);
}