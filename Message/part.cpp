#include "Message.hpp"

/**
 * @brief 채널과 이유를 파싱하는 함수입니다.
 *
 * @param params 파라미터 벡터로, 첫 번째 요소는 채널 이름들이 쉼표로 구분된 문자열입니다.
 *               두 번째 요소부터는 이유로 처리됩니다.
 * @param channels 파싱된 채널 이름들이 저장될 벡터입니다.
 * @param reason 파싱된 이유가 저장될 문자열입니다.
 */
static void parseChannelAndReason(const std::vector<std::string> &params, std::vector<std::string> &channels,
                                  std::string &reason)
{
    std::istringstream iss(params[0]);
    std::string channelName;
    while (std::getline(iss, channelName, ','))
    {
        channels.push_back(channelName);
    }
    // 두 번째 파라미터부터는 이유로 처리
    for (size_t i = 1; i < params.size(); ++i)
    {
        if (i > 1)
            reason += " ";
        reason += params[i];
    }
}

/**
 * @brief Message 클래스의 partExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어를 인자로 받아서
 * 클라이언트가 채널에서 나가는 동작을 수행합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
 */
void Message::partExecute(Server &server, Client &client, Command *cmd)
{
    std::vector<std::string> channels;
    std::string reason;

    if (cmd->getParams().empty()) // /part     /part #channel
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, "PART"));
        return;
    }
    parseChannelAndReason(cmd->getParams(), channels, reason);

    std::vector<std::string>::iterator it_channel;
    for (it_channel = channels.begin(); it_channel != channels.end(); ++it_channel)
    {
        std::string channelName = *it_channel;
        Channel *channel = server.findChannel(channelName);

        if (channel == NULL)
        {
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, channelName));
            continue;
        }
        if (!channel->isMember(client))
        {
            client.addSendMsg(Response::ERR_NOTONCHANNEL_442(server, client, *channel));
            continue;
        }
        if (!reason.empty())
            channel->addSendMsgAll(server, client.getClientPrefix(), "PART", channelName, reason);
        else
            channel->addSendMsgAll(server, client.getClientPrefix(), "PART", "", channelName);
        channel->partChannel(client);
    }
}