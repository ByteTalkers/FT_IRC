#include "Message.hpp"

static bool validCheck(const std::string &nick);
static bool checkChar(char c);

/**
 * 유저 등록 전 NICK 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 닉네임 유효하지 않으면 => ERR_ERRONEUSNICKNAME_432
 * - 닉네임 중복이면 => ERR_NICKNAMEINUSE_433
 * - 클라이언트에 닉네임 세팅
 * - 클라이언트 등록 닉네임 플래그 켜기
 */

/**
 * @brief Message 클래스의 registerNickExecute 함수는 서버와 클라이언트 간의 닉네임 등록을 처리합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 수신된 명령어 객체
 */
void Message::registerNickExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    if (cmd->getParamsCount() != 1 || !validCheck(cmd->getParams()[0]))
    {
        std::string nicks = "";
        for (size_t i = 0; i < cmd->getParamsCount(); ++i)
        {
            if (i != 0)
            {
                nicks += " ";
            }
            nicks += cmd->getParams()[i];
        }
        client.addSendMsg(Response::ERR_ERRONEUSNICKNAME_432(server, client, nicks));
        return;
    }
    if (server.searchNick(client.getsockfd(), cmd->getParams()[0]))
    {
        client.addSendMsg(Response::ERR_NICKNAMEINUSE_433(server, client, cmd->getParams()[0]));
        return;
    }

    client.setNick(cmd->getParams()[0]);
    if (!client.getRegisterd())
    {
        client.setRegisterFlags(NICK_REG, true);
    }
    else
    {
        if (client.getCurChannel().empty())
        {
            client.addSendMsg(Response::GENERATE(client.getClientPrefix(), "NICK", ":" + client.getNick()));
            return;
        }
        Channel *channel;
        std::map<std::string, Channel *>::iterator it;
        for (it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
        {
            channel = (*it).second;
            if (channel->isMember(client))
            {
                channel->addSendMsgAll(server, client.getClientPrefix(), "NICK", "", client.getNick());
            }
        }
    }
}

/**
 * 유저 등록 후 NICK 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 닉네임 유효하지 않으면 => ERR_ERRONEUSNICKNAME_432
 * - 닉네임 중복이면 => ERR_NICKNAMEINUSE_433
 * - 클라이언트에 닉네임 세팅
 */

/**
 * @brief Message 클래스의 nickExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어를 인자로 받아서 실행합니다.
 *
 * @param server 서버 객체입니다.
 * @param client 클라이언트 객체입니다.
 * @param cmd 명령어 객체입니다.
 */
void Message::nickExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    if (!validCheck(cmd->getParams()[0]))
    {
        client.addSendMsg(Response::ERR_ERRONEUSNICKNAME_432(server, client, cmd->getParams()[0]));
        return;
    }
    client.setNick(cmd->getParams()[0]);
}

/**
 * 닉네임 유효성 검사
 * - 길이 9자 제한
 * - 채널명 prefix '#', '&' 첫글자에 금지
 * - 알파벳, 숫자, 중괄호 {}, 대괄호 [], 파이프 | 기호만 가능
 */

/**
 * @brief 주어진 닉네임이 유효한지 확인하는 함수입니다.
 *
 * @param nick 확인할 닉네임
 * @return 유효한 닉네임인 경우 true, 그렇지 않은 경우 false를 반환합니다.
 */
static bool validCheck(const std::string &nick)
{
    if (nick.length() > 9)
    {
        return false;
    }
    if (nick[0] == '#' || nick[0] == '&')
    {
        return false;
    }

    for (size_t i = 0; i < nick.length(); i++)
    {
        if (!checkChar(nick[i]))
        {
            return false;
        }
    }
    return true;
}

static bool checkChar(char c)
{
    if (!std::isalnum(c) && c != '[' && c != ']' && c != '{' && c != '}' && c != '|')
    {
        return false;
    }
    return true;
}