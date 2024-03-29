#include "Message.hpp"

/**
 * @brief 클라이언트를 채널에서 강제로 퇴장시키는 함수입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령 객체
 */
void Message::kickExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    std::string channelName = cmd->getParams()[0];
    std::string targetNick = cmd->getParams()[1];
    std::string reason = cmd->getParamsCount() > 2 ? cmd->getParams()[2] : "";

    Channel *channel = server.findChannel(channelName);
    if (!channel)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channelName));
        return;
    }

    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }

    Client *targetClient = server.findClient(targetNick);
    if (!targetClient)
    {
        client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
        return;
    }
    if (!channel->isMember(*targetClient))
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, targetNick));
        return;
    }

    channel->addSendMsgAll(server, client.getClientPrefix(), "KICK", channelName + " " + targetNick, reason);
    channel->partChannel(*targetClient);
}