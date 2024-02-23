#include "Message.hpp"

/**
 * topic 실행부
 * 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * 해당 채널이 없으면 => ERR_NOSUCHCHANNEL_403
 * 파라미터로 채널명만 들어온 경우는 토픽 확인만 => 토픽 존재 시 RPL_TOPIC_332, 토픽 없으면 RPL_NOTOPIC_331
 * 해당 채널에 없는 유저가 토픽 세팅하려는 경우 => ERR_NOTONCHANNEL_442
 * 채널명 + 토픽내용이면 토픽 세팅
 * 채널의 토픽 설정이 운영자 전용 + 해당 클라이언트가 운영자가 아니면 => ERR_CHANOPRIVSNEEDED_482
 * (채널의 토픽 설정이 운영자 전용 + 해당 클라이언트 운영자) or (채널에 토픽 설정이 false) => 토픽 설정 후 메시지
 */
void Message::topicExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    std::string channel_name = cmd->getParams()[0];
    Channel *channel = server.findChannel(channel_name);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getParams()[0]));
        return;
    }

    if (cmd->getParamsCount() == 1)
    {
        if (channel->getTopicExist())
        {
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, *channel));
            return;
        }
        client.addSendMsg(Response::RPL_NOTOPIC_331(server, client, *channel));
        return;
    }
 
    if (!channel->isMember(client))
    {
        client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
        return;
    }

    if (channel->getModeTopic() && !channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if ((channel->getModeTopic() && channel->checkOp(client)) || !channel->getModeTopic())
    {
        channel->setTopicExist(true);
        channel->setTopic(cmd->getParams()[1]);
        channel->addSendMsgAll(server, client.getNick(), "TOPIC", cmd->getParams()[1]);
    }
}
