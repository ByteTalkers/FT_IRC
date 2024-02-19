#include "Message.hpp"

void Message::topicExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 있다면,
    if (cmd->getParamsCount() > 0)
    {
        std::string channel_name;
        channel_name = cmd->getParams()[0];

        Channel *channel = server.findChannel(channel_name);
        // 해당 채널 있음
        if (channel != NULL)
        {
            // 파라미터 채널명만 들어온 경우 => 토픽 확인
            if (cmd->getParamsCount() == 1)
            {
                // 토픽 존재여부 확인
                if (channel->getSetTopic())
                {
                    client.addSendMsg(
                        Response::rplTopic_332(server.getName(), client.getNick(), channel_name, channel->getTopic()));
                }
                else
                {
                    client.addSendMsg(Response::rplNoTopic_331(server.getName(), client.getNick(), channel_name));
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
                    }
                    else
                    {
                        client.addSendMsg(
                            Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
                else
                {
                    channel->setTopic(cmd->getParams()[1]);
                }
            }
        }
        // 없으면 no such 채널
        else
        {
            client.addSendMsg(Response::errNoSuchChannel_403(server.getName(), client.getNick(), channel_name));
        }
    }
    // 파라미터 없다면, needmoreparameter 에러
    else
    {
        client.addSendMsg(Response::errNeedMoreParams_461(server.getName(), client.getNick(), cmd->getCommand()));
    }
}
