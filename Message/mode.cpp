#include "Message.hpp"

void Message::modeExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    (void)cmd;
    client.addSendMsg(Response::rplUmodeIs_221(client.getUser(), client.getNick()));
}