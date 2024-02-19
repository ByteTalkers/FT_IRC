#include "Message.hpp"

void Message::passExecute(Server &server, Client &client)
{
    (void)client;
    if (server.getPassword() != this->m_params[0])
    {
        //err
    }
}