#include "Message.hpp"

void Message::topicExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 없음
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }

    std::string channel_name;
    channel_name = cmd->getParams()[0];
    Channel *channel = server.findChannel(channel_name);
    // 해당 채널 없음
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, *channel));
        client.setWriteTypes(MYSELF);
        return;
    }

    // 파라미터 채널명만 들어온 경우 => 토픽 확인
    if (cmd->getParamsCount() == 1)
    {
        // 토픽 존재여부 확인
        if (channel->getSetTopic())
        {
            client.addSendMsg(
                Response::RPL_TOPIC_332(server, client, *channel));
            client.setWriteTypes(MYSELF);
        }
        else
        {
            client.addSendMsg(Response::RPL_NOTOPIC_331(server, client, *channel));
            client.setWriteTypes(MYSELF);
        }
    }
    // 채널명 + 인자인 경우 => 토픽 세팅
    else
    {
        // 채널의 토픽 설정 운영자 전용인지 아닌지 확인
        if (channel->getModeTopic())
        {
            // 채널 op인지 확인
            if (channel->checkOp(client))
            {
                channel->setTopic(cmd->getParams()[1]);
                channel->addSendMsgAll(server, client.getNick(), "TOPIC", cmd->getParams()[1]);
                client.setWriteTypes(EVERYONE);
            }
            else
            {
                client.addSendMsg(
                    Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
                client.setWriteTypes(MYSELF);
            }
        }
        else
        {
            channel->setTopic(cmd->getParams()[1]);
            channel->addSendMsgAll(server, client.getNick(), "TOPIC", cmd->getParams()[1]);
            client.setWriteTypes(EVERYONE);
        }
    }
}
