#include "Message.hpp"

/**
 * @brief Message 클래스의 topicExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어(cmd)를 인자로 받아서 실행합니다.
 * 주어진 명령어의 파라미터 개수를 확인하고, 필요한 파라미터가 부족한 경우 에러 메시지를 클라이언트에게 전송합니다.
 * 그렇지 않은 경우, 주어진 채널 이름을 사용하여 서버에서 해당 채널을 찾고, 채널이 존재하지 않는 경우 에러 메시지를
 * 클라이언트에게 전송합니다.
 *
 * 파라미터 개수가 1개인 경우, 채널의 토픽이 존재하는지 확인하고, 존재하는 경우 해당 토픽을 클라이언트에게 전송합니다.
 * 토픽이 존재하지 않는 경우, 해당 정보를 클라이언트에게 전송합니다.
 *
 * 파라미터 개수가 2개인 경우, 클라이언트가 채널의 멤버인지 확인하고, 멤버가 아닌 경우 에러 메시지를 클라이언트에게
 * 전송합니다. 채널의 토픽 모드가 설정되어 있고, 클라이언트가 채널의 오퍼레이터 권한을 가지고 있지 않은 경우 에러
 * 메시지를 클라이언트에게 전송합니다. 그렇지 않은 경우, 채널의 토픽을 설정하고, 해당 정보를 모든 클라이언트에게
 * 전송합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
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
        channel->addSendMsgAll(server, client.getClientPrefix(), "TOPIC", channel_name, cmd->getParams()[1]);
    }
}
