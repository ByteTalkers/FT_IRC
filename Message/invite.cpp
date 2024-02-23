#include "Message.hpp"

/**
 * INVITE 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 채널 없으면 => ERR_NOSUCHCHANNEL_403
 * - 초대받은 유저가 존재하지 않는 유저면 => ERR_NOSUCHNICK_401
 * - 클라이언트가 해당 채널에 들어가 있지 않으면 => ERR_NOTONCHANNEL_442
 * - 클라이언트가 채널 op가 아니면 => ERR_CHANOPRIVSNEEDED_482
 * - 초대받은 유저가 이미 들어가 있으면 => ERR_USERONCHANNEL_443
 * - 채널 초대목록에 추가
 */
void Message::inviteExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    std::string channel_name = cmd->getParams()[1];
    Channel *channel = server.findChannel(channel_name);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channel_name));
        return;
    }
    Client *invited = server.findClient(cmd->getParams()[0]);
    if (invited == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, cmd->getParams()[0]));
        return;
    }
    if (!channel->isMember(client))
    {
        client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
        return;
    }
    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if (channel->isMember(*invited))
    {
        client.addSendMsg(Response::ERR_USERONCHANNEL_443(server, client, *invited, *channel));
        return;
    }
    channel->addInvitation(invited->getNick());
    client.addSendMsg(Response::RPL_INVITING_341(server, client, *invited, *channel));
    invited->addSendMsg(Response::GENERATE(client.getNick(), "INVITE", invited->getNick() + " :" + channel_name));
    server.enableWriteEvent(invited->getsockfd());
}
