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
 * Executes the REGISTER NICK command.
 * This function is responsible for registering a nickname for a client on the server.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object containing the parameters.
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
 * Executes the NICK command.
 * Sets the nickname of the client to the specified nickname.
 * If the command parameters are invalid or insufficient, an error response is sent to the client.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object representing the NICK command.
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
 * Checks if a given nickname is valid.
 * A valid nickname must satisfy the following conditions:
 * - The length of the nickname must not exceed 9 characters.
 * - The first character of the nickname must not be '#' or '&'.
 * - All characters in the nickname must pass the checkChar function.
 *
 * @param nick The nickname to be checked.
 * @return True if the nickname is valid, false otherwise.
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

/**
 * Checks if a character is alphanumeric or one of the following special characters: [ ] { } |
 *
 * @param c The character to be checked
 * @return true if the character is alphanumeric or one of the special characters, false otherwise
 */
static bool checkChar(char c)
{
    if (!std::isalnum(c) && c != '[' && c != ']' && c != '{' && c != '}' && c != '|')
    {
        return false;
    }
    return true;
}