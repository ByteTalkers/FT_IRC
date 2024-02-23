#include "Message.hpp"

/**
 * 채널 입장 시 요청하는 WHO #채널명 응답
 * 인자 부족하면 => ERR_NEEDMOREPARAMS_461
 * 없는 채널명 => 
*/

void Message::whoExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }
    Channel *channel = server.findChannel(cmd->getParams()[0]);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }

    client.addSendMsg(Response::RPL_WHOREPLY_352(server, client, *channel));
    client.addSendMsg(Response::RPL_ENDOFWHO_315(server, client, *channel));
    client.setWriteTypes(MYSELF);
}