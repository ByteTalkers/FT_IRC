#include "Message.hpp"

void Message::whoisExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    (void)cmd;
    client.addSendMsg(Response::rplWhoIsUser_311(server.getName(), client.getNick(), client.getUser()));
    client.addSendMsg(Response::rplWhoIsServer_312(server.getName(), client.getNick()));
    client.addSendMsg(Response::rplEndofWhoIs_318(server.getName(), client.getNick()));
}