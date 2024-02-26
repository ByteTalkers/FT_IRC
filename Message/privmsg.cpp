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
 * 나머지 파라미터들 스페이스 추가해서 합치는 함수
 */
static const std::string makeMsg(const std::vector<std::string> &params)
{
    std::string msg = "";
    for (std::size_t i = 1; i < params.size(); i++)
    {
        msg += params[i];
        if (i != params.size() - 1)
        {
            msg += " ";
        }
    }
    return msg;
}

/**
 * @lotto 명령어를 실행하는 함수
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
 * 채널에 메시지를 보내는 함수
 */
static void sendPrivmsgToChannel(Server &server, Client &client, const std::vector<std::string> &params)
{
    bool isLotto = false;
    std::string lottoMsg = "Lotto numbers: ";
    if (params.size() > 1 && params[1] == "@lotto")
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
    if (isLotto)
    {
        receiver->addSendMsgAll(server, client.getNick(), "PRIVMSG", receiver->getName(), lottoMsg);
        client.addSendMsg(
            Response::GENERATE(client.getNick(), "PRIVMSG", receiver->getName() + " :" + lottoMsg).c_str());
        server.enableWriteEvent(client.getsockfd());
    }
    receiver->addSendMsgAll(server, client.getNick(), "PRIVMSG", receiver->getName(), makeMsg(params));
}

/**
 * 해당 유저의 send버퍼에 메시지를 담아주고, write이벤트를 켜주는 함수
 */
static void sendPrivmsgToClient(Server &server, Client &client, const std::vector<std::string> &params)
{
    Client *receiver = server.findClient(params[0]);
    client.setRecvFd(receiver->getsockfd());
    receiver->addSendMsg(
        Response::GENERATE(client.getNick(), "PRIVMSG", receiver->getNick() + " :" + makeMsg(params)).c_str());
    server.enableWriteEvent(receiver->getsockfd());
}

/**
 * 콤마(,) 기준으로 스플릿하는 함수
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
 * 채널인지, 유저인지 확인하는 함수
 */
static ePrivmsg validCheck(Server &server, const std::string &to)
{
    if (to[0] == '&' || to[0] == '#')
    {
        Channel *channel = server.findChannel(to);
        if (channel != NULL)
        {
            return CHANNEL;
        }
        return NOCHANNEL;
    }
    else
    {
        Client *client = server.findClient(to);
        if (client != NULL)
        {
            return CLIENT;
        }
        return NOCLIENT;
    }
}