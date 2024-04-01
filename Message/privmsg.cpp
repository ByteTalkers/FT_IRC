#include "Message.hpp"

enum ePrivmsg
{
    CHANNEL,
    CLIENT,
    NOCHANNEL,
    NOCLIENT,
};

static std::vector<std::string> splitComma(const std::string &command);
static ePrivmsg validCheck(Server &server, const std::string &to);
static void sendPrivmsgToChannel(Server &server, Client &client, const std::vector<std::string> &params);
static void sendPrivmsgToClient(Server &server, Client &client, const std::vector<std::string> &params);

/**
 * privmsg 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 첫번째 파라미터 => 받는 사람 or 채널
 * - 채널 대상인 경우, 보내는 클라이언트가 채널에 포함됐는지 확인 => ERR_CANNOTSENDTOCHAN_404
 * - 없는 채널 => ERR_NOSUCHCHANNEL_403
 * - 없는 유저 => ERR_NOSUCHNICK_401
 * - 메시지는 남은 파라미터들 사이에 스페이스 추가해서 합쳐줌
 */

/**
 * Executes the PRIVMSG command.
 * This function sends a message to one or more recipients, either to a channel or to a specific client.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object containing the PRIVMSG command and its parameters.
 */
void Message::privmsgExecute(Server &server, Client &client, Command *cmd)
{

    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    std::vector<std::string> to = splitComma(cmd->getCommand());
    std::vector<std::string>::iterator it;
    for (it = to.begin(); it < to.end(); it++)
    {
        ePrivmsg to = validCheck(server, cmd->getParams()[0]);
        std::string channel_name;
        channel_name = cmd->getParams()[0];
        Channel *channel = NULL;
        switch (to)
        {
        case CHANNEL:
            channel = server.findChannel(channel_name);
            if (!channel->isMember(client))
            {
                client.addSendMsg(Response::ERR_CANNOTSENDTOCHAN_404(server, client, *channel));
                break;
            }
            sendPrivmsgToChannel(server, client, cmd->getParams());
            break;
        case CLIENT:
            sendPrivmsgToClient(server, client, cmd->getParams());
            break;
        case NOCHANNEL:
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getParams()[0]));
            break;
        case NOCLIENT:
            client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, cmd->getParams()[0]));
            break;
        }
    }
}

/**
 * @brief Executes the Lotto function.
 *
 * This function performs the execution of the Lotto function and returns a vector of integers.
 *
 * @return std::vector<int> - A vector of integers.
 */
static std::vector<int> executeLotto(void)
{
    std::vector<int> numbers;
    while (numbers.size() < 6)
    {
        int num = rand() % 45 + 1;
        if (std::find(numbers.begin(), numbers.end(), num) == numbers.end())
            numbers.push_back(num);
    }
    std::sort(numbers.begin(), numbers.end());
    return numbers;
}

/**
 * Sends a private message to a channel.
 *
 * This function sends a private message to a specified channel. If the message is a lotto command,
 * it generates lotto numbers and appends them to the message before sending it to the channel.
 *
 * @param server The server object.
 * @param client The client object.
 * @param params The vector of parameters containing the channel name and the message.
 */
static void sendPrivmsgToChannel(Server &server, Client &client, const std::vector<std::string> &params)
{
    bool isLotto = false;
    std::string lottoMsg = "Lotto numbers: ";
    if (params.size() > 1 && params[1] == LOTTO)
    {
        isLotto = true;
        std::vector<int> lottoNumbers = executeLotto();
        for (size_t i = 0; i < lottoNumbers.size(); ++i)
        {
            lottoMsg += std::to_string(lottoNumbers[i]);
            if (i != lottoNumbers.size() - 1)
                lottoMsg += ", ";
        }
    }
    Channel *receiver = server.findChannel(params[0]);
    receiver->addSendMsgAll(server, client.getClientPrefix(), "PRIVMSG", receiver->getName(), params[1]);
    if (isLotto)
    {
        receiver->addSendMsgAll(server, client.getClientPrefix(), "PRIVMSG", receiver->getName(), lottoMsg);
        client.addSendMsg(
            Response::GENERATE(client.getClientPrefix(), "PRIVMSG", receiver->getName() + " :" + lottoMsg).c_str());
        server.enableWriteEvent(client.getsockfd());
    }
}

/**
 * Sends a private message from one client to another.
 *
 * @param server The server object.
 * @param client The client sending the message.
 * @param params The parameters of the message, where params[0] is the recipient's nickname and params[1] is the message
 * content.
 */
static void sendPrivmsgToClient(Server &server, Client &client, const std::vector<std::string> &params)
{
    Client *receiver = server.findClient(params[0]);
    client.setRecvFd(receiver->getsockfd());
    receiver->addSendMsg(
        Response::GENERATE(client.getClientPrefix(), "PRIVMSG", receiver->getNick() + " :" + params[1]).c_str());
    server.enableWriteEvent(receiver->getsockfd());
}

/**
 * @brief Splits a command string by comma and returns a vector of strings.
 *
 * This function takes a command string as input and splits it by comma. It then
 * stores each split substring into a vector of strings and returns the vector.
 *
 * @param command The command string to be split.
 * @return std::vector<std::string> A vector of strings containing the split substrings.
 */
static std::vector<std::string> splitComma(const std::string &command)
{
    std::vector<std::string> ret;
    std::istringstream iss(command);
    std::string buffer;

    while (getline(iss, buffer, ' '))
    {
        ret.push_back(buffer);
    }
    return ret;
}

/**
 * @enum ePrivmsg
 * Enumerates the possible types of private messages.
 *
 * @var CHANNEL Represents a private message to a channel.
 * @var NOCHANNEL Represents a private message to a non-existent channel.
 * @var CLIENT Represents a private message to a client.
 * @var NOCLIENT Represents a private message to a non-existent client.
 */
static ePrivmsg validCheck(Server &server, const std::string &to)
{
    if (to[0] == '&' || to[0] == '#')
    {
        Channel *channel = server.findChannel(to);
        if (channel != NULL)
        {
            return ePrivmsg::CHANNEL;
        }
        return ePrivmsg::NOCHANNEL;
    }
    else
    {
        Client *client = server.findClient(to);
        if (client != NULL)
        {
            return ePrivmsg::CLIENT;
        }
        return ePrivmsg::NOCLIENT;
    }
}