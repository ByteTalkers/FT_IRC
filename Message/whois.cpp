#include "Message.hpp"

void Message::whoisExecute(Server &server, Client &client, Command *cmd)
{
    (void)server;
    (void)cmd;
    client.addSendMsg(Response::RPL_WHOISUSER_311(server, client));
    client.addSendMsg(Response::RPL_WHOISSERVER_312(server, client));
    client.addSendMsg(Response::RPL_ENDOFWHOIS_318(server, client));
    client.setWriteTypes(MYSELF);
}