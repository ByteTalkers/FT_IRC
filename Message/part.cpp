#include "Message.hpp"

/**
 * Parses the channel and reason from the given parameters.
 *
 * @param params The vector of strings containing the parameters.
 * @param channels The vector of strings to store the parsed channels.
 * @param reason The string to store the parsed reason.
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
 * Executes the PART command for a client in the server.
 *
 * This function is responsible for handling the PART command, which allows a client to leave one or more channels.
 * It checks the parameters provided by the client and performs the necessary actions based on the channel and reason.
 * If the parameters are invalid or missing, an error response is sent to the client.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object containing the parameters.
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