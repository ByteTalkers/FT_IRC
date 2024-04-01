#include "Message.hpp"

/**
 * Parses the given parameter and extracts channel names.
 *
 * @param param The parameter to parse.
 * @param channelNames The vector to store the extracted channel names.
 */
static void parseChannelParams(const std::string &param, std::vector<std::string> &channelNames)
{
    std::istringstream iss(param);
    std::string channelToken;
    while (std::getline(iss, channelToken, ','))
    {
        channelNames.push_back(channelToken);
    }
}

/**
 * @brief Parses the key parameters from the given string and stores them in the provided vector.
 *
 * This function takes a string parameter and a vector reference as input. It parses the key parameters
 * from the string and stores them in the vector. The key parameters are separated by a delimiter.
 *
 * @param param The string containing the key parameters.
 * @param keys The vector to store the parsed key parameters.
 */
static void parseKeyParams(const std::string &param, std::vector<std::string> &keys)
{
    std::istringstream keyIss(param);
    std::string keyToken;
    while (std::getline(keyIss, keyToken, ','))
    {
        keys.push_back(keyToken);
    }
}

/**
 * @brief Checks the invite mode for a client joining a channel.
 *
 * This function is responsible for checking the invite mode for a client joining a channel.
 * It takes references to the `Server`, `Client`, and `Channel` objects, as well as a reference
 * to a `std::string` representing the invite key. The function performs the necessary checks
 * and updates the channel's state accordingly.
 *
 * @param server The reference to the `Server` object.
 * @param client The reference to the `Client` object.
 * @param channel The reference to the `Channel` object.
 * @param key The reference to the invite key.
 */
static void checkInviteMode(Server &server, Client &client, Channel &channel, std::string &key)
{
    if (!channel.isInvited(client.getNick()))
    {
        // 키 체크
        if (!channel.checkKey(key))
        {
            client.addSendMsg(Response::ERR_BADCHANNELKEY_475(server, client, channel));
            return;
        }
        // 인원 수 체크
        if (channel.getModeLimit() && channel.getLimitCount() <= channel.getUserCount())
        {
            client.addSendMsg(Response::ERR_CHANNEL_IS_FULL_471(server, client, channel));
            return;
        }
        client.addSendMsg(Response::ERR_INVITEONLYCHAN_473(server, client, channel));
    }
    else
    {
        // 채널 초대 목록에서 제거
        channel.removeInvitation(client.getNick());
        // 채널에 멤버 추가
        channel.addMember(&client);
        channel.addSendMsgAll(server, client.getClientPrefix(), "JOIN ", "", channel.getName());
        if (channel.getTopicExist())
        {
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, channel));
        }
        client.addSendMsg(Response::RPL_NAMREPLY_353(server, client, channel));
        client.addSendMsg(Response::RPL_ENDOFNAMES_366(server, client, channel));
    }
}

/**
 * Executes the JOIN command for a client to join one or more channels.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The JOIN command object.
 */
void Message::joinExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        // 채널 이름이 없는 경우 에러 메시지 전송(Irssi: Not enough parameters given)
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    std::vector<std::string> channelNames;
    std::vector<std::string> keys;

    parseChannelParams(cmd->getParams()[0], channelNames);
    if (cmd->getParams().size() > 1)
        parseKeyParams(cmd->getParams()[1], keys);

    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        std::string channelName = channelNames[i];
        if (channelName[0] != '#' && channelName[0] != '&')
        {
            client.addSendMsg(Response::ERR_BADCHANMASK_476(server, client, channelName));
            return;
        }
        std::string key = i < keys.size() ? keys[i] : "";

        Channel *channel = server.findChannel(channelName);
        bool isNewChannel = false;
        if (channel == NULL)
        {
            channel = new Channel(channelName);
            server.addChannel(channelName, channel);
            isNewChannel = true;
            if (key != "")
            {
                channel->setKey(keys[i]);
                channel->setModeKey(true);
            }
        }

        if (channel->isMember(client))
        {
            // 에러메시지 잘 모르겠음 (irc.example.com 443 nana nana_ #a :is already on channel) -> nana 와 nana_
            // 구분안됨
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, *channel));
            client.addSendMsg(Response::RPL_NAMREPLY_353(server, client, *channel));
            client.addSendMsg(Response::RPL_ENDOFNAMES_366(server, client, *channel));
            /*이렇게 하면 사용자가 이미 속해 있는 채널에 다시 JOIN 명령을 시도했을 때, 채널의 현재 상태 정보를 다시
             * 제공하여 사용자가 이미 채널에 있다는 사실을 간접적으로 알릴 수 있습니다.*/
            return;
        }
        if (channel->getModeInvite()) // 초대 모드
        {
            checkInviteMode(server, client, *channel, key);
            continue;
        }
        // 키 체크
        if (channel->getModeKey() && !channel->checkKey(key))
        {
            client.addSendMsg(Response::ERR_BADCHANNELKEY_475(server, client, *channel));
            continue;
        }
        // 채널 인원 수 체크
        if (channel->getModeLimit() && channel->getLimitCount() <= channel->getUserCount())
        {
            client.addSendMsg(Response::ERR_CHANNEL_IS_FULL_471(server, client, *channel));
            continue;
        }

        channel->addMember(&client);
        if (isNewChannel)
        {
            channel->addOperator(client.getNick());
        }
        channel->addSendMsgAll(server, client.getClientPrefix(), "JOIN ", "", channelName);
        if (channel->getTopicExist())
        {
            client.addSendMsg(Response::RPL_TOPIC_332(server, client, *channel));
        }
        client.addSendMsg(Response::RPL_NAMREPLY_353(server, client, *channel));
        client.addSendMsg(Response::RPL_ENDOFNAMES_366(server, client, *channel));
    }
}
