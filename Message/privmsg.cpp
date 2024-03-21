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
 * @brief privmsgExecute 함수는 PRIVMSG 명령을 처리하는 메서드입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령 객체
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
 * @brief 로또 번호를 생성하여 반환하는 함수입니다.
 *
 * @return std::vector<int> 생성된 로또 번호를 담고 있는 벡터
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
 * @brief 특정 채널에 PRIVMSG를 보내는 함수입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param params PRIVMSG를 보낼 때 필요한 매개변수들의 벡터
 *
 * @details 이 함수는 주어진 채널에 PRIVMSG를 보내는 역할을 합니다.
 * 만약 params의 크기가 2 이상이고 params[1]이 "LOTTO"와 같다면,
 * 로또 번호를 생성하여 PRIVMSG에 추가합니다.
 *
 * @note 이 함수는 server 객체에서 주어진 채널을 찾아서 PRIVMSG를 보냅니다.
 * 만약 로또 번호가 추가되었다면, 해당 채널과 클라이언트에게 추가적인 메시지를 보냅니다.
 *
 * @see Server, Client, Channel
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
 * @brief 클라이언트에게 PRIVMSG 메시지를 보내는 함수입니다.
 *
 * @param server 서버 객체
 * @param client 보내는 클라이언트 객체
 * @param params PRIVMSG 메시지의 매개변수로 사용될 벡터
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
 * @brief 문자열을 쉼표로 분리하여 std::vector<std::string>으로 반환하는 함수입니다.
 *
 * @param command 분리할 문자열
 * @return 쉼표로 분리된 문자열들을 담은 std::vector<std::string>
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
 * @brief 주어진 서버와 수신자(to)를 기반으로 유효성을 검사합니다.
 *
 * @param server 서버 객체
 * @param to 수신자
 * @return ePrivmsg 유효성 검사 결과
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