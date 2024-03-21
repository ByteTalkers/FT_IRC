#include "Message.hpp"

enum eModes
{
    IMODE,
    TMODE,
    KMODE,
    OMODE,
    LMODE,
    UNKNOWN
};

static eModes checkMode(char c);
static void modeI(Server &server, Client &client, Channel *channel, bool mode_flag);
static void modeT(Server &server, Client &client, Channel *channel, bool mode_flag);
static void modeK(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &key);
static void modeO(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &nick);
static void modeL(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &limit);

/**
 * mode 실행부
 * - channel => i, t, k, o, l 옵션만 허용
 * 1. 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * 2. 유저명이 들어온 경우
 * - 해당 유저명 없으면 => ERR_NOSUCHNICK_401
 * - 해당 유저가 요청 클라이언트와 다를 때, 파라미터가 있으면 => ERR_UMODEUNKNWONFLAG_501
 * - 해당 유저가 요청 클라이언트와 다를 때, 파라미터가 없으면 => ERR_USERSDONTMATCH_502
 * - 해당 유저와 요청 클라이언트가 같으면 => RPL_UMODEIS_221
 * 3. 해당 채널명 없으면 => ERR_NOSUCHCHANNEL_403
 * 4. 파라미터 하나인 경우 => RPL_CHANNELMODEIS_324, RPL_CREATIONTIME_329
 * 5. 해당 채널의 op가 아니면 => ERR_CHANOPRIVSNEEDED_482
 */

/**
 * @brief Message 클래스의 modeExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어(cmd)를 인자로 받아서 실행합니다.
 * modeExecute 함수는 주어진 명령어에 따라 채널 모드를 설정하거나 유저 모드를 변경합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
 */
void Message::modeExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }

    if (cmd->getParams()[0][0] != '#' && cmd->getParams()[0][0] != '&')
    {
        Client *find_client = server.findClient(cmd->getParams()[0]);
        if (find_client == NULL)
        {
            client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, cmd->getParams()[0]));
            return;
        }
        if (cmd->getParamsCount() > 1)
        {
            for (size_t i = 1; i < cmd->getParamsCount(); i++)
            {
                for (size_t j = 0; j < cmd->getParams()[i].length(); j++)
                {
                    if (cmd->getParams()[i][j] != '+' && cmd->getParams()[i][j] != '-')
                    {
                        client.addSendMsg(
                            Response::ERR_UMODEUNKNWONFLAG_501(server, client, std::string(1, cmd->getParams()[i][j])));
                    }
                }
            }
            return;
        }
        if (find_client != &client)
        {
            client.addSendMsg(Response::ERR_USERSDONTMATCH_502(server, client));
            return;
        }
        client.addSendMsg(Response::RPL_UMODEIS_221(client));
        return;
    }

    std::string channel_name;
    channel_name = cmd->getParams()[0];
    Channel *channel = server.findChannel(channel_name);

    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getParams()[0]));
        return;
    }

    if (cmd->getParamsCount() == 1)
    {
        client.addSendMsg(Response::RPL_CHANNELMODEIS_324(server, client, *channel));
        client.addSendMsg(Response::RPL_CREATIONTIME_329(server, client, *channel));
        return;
    }

    std::string modes = cmd->getParams()[1];
    bool mode_flag = true;
    size_t param_idx = 2;

    for (size_t i = 0; i < modes.length(); i++)
    {
        if (modes[i] == '+')
        {
            mode_flag = true;
            continue;
        }
        if (modes[i] == '-')
        {
            mode_flag = false;
            continue;
        }

        switch (checkMode(modes[i]))
        {
        case IMODE:
            modeI(server, client, channel, mode_flag);
            break;
        case TMODE:
            modeT(server, client, channel, mode_flag);
            break;
        case KMODE:
            modeK(server, client, channel, mode_flag,
                  param_idx < cmd->getParamsCount() ? cmd->getParams()[param_idx] : "");
            if (param_idx < cmd->getParamsCount())
            {
                param_idx++;
            }
            break;
        case OMODE:
            modeO(server, client, channel, mode_flag,
                  param_idx < cmd->getParamsCount() ? cmd->getParams()[param_idx] : "");
            if (param_idx < cmd->getParamsCount())
            {
                param_idx++;
            }
            break;
        case LMODE:
            modeL(server, client, channel, mode_flag,
                  param_idx < cmd->getParamsCount() ? cmd->getParams()[param_idx] : "");
            if (param_idx < cmd->getParamsCount())
            {
                param_idx++;
            }
            break;
        case UNKNOWN:
            client.addSendMsg(Response::ERR_UNKNOWNMODE_472(server, client, std::string(1, modes[i])));
            break;
        }
    }
}

static eModes checkMode(char c)
{
    if (c == 'i')
    {
        return IMODE;
    }
    else if (c == 't')
    {
        return TMODE;
    }
    else if (c == 'k')
    {
        return KMODE;
    }
    else if (c == 'o')
    {
        return OMODE;
    }
    else if (c == 'l')
    {
        return LMODE;
    }
    return UNKNOWN;
}

/**
 * 인자가 +i && 해당 채널이 Invite 모드가 아닐 때
 * 인자가 -i && 해당 채널이 Invite 모드일 때
 */
static void modeI(Server &server, Client &client, Channel *channel, bool mode_flag)
{
    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if (mode_flag && !channel->getModeInvite())
    {
        channel->setModeInvite(true);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName(), "+i");
    }
    if (!mode_flag && channel->getModeInvite())
    {
        channel->setModeInvite(false);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName(), "-i");
    }
}

/**
 * 인자가 +t && 해당 채널이 Topic 모드가 아닐 때
 * 인자가 -t && 해당 채널이 Topic 모드일 때
 */
static void modeT(Server &server, Client &client, Channel *channel, bool mode_flag)
{
    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if (mode_flag && !channel->getModeTopic())
    {
        channel->setModeTopic(true);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName(), "+t");
    }
    if (!mode_flag && channel->getModeTopic())
    {
        channel->setModeTopic(false);
        channel->setTopicExist(false);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName(), "-t");
    }
}

/**
 * 인자가 +k && 해당 채널이 Key 모드가 아닐 때
 * 인자가 -k && 해당 채널이 Key 모드일 때
 */

static void modeK(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &key)
{
    if (mode_flag && key == "")
    {
        client.addSendMsg(Response::ERR_SPECIFYPARAMETER_696(server, client, *channel, "key"));
        return;
    }
    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    if (mode_flag && !channel->getModeKey())
    {
        channel->setModeKey(true);
        channel->setKey(key);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName() + " +k", key);
    }
    if (!mode_flag && channel->getModeKey())
    {
        channel->setModeKey(false);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName() + " -k", channel->getKey());
        channel->setKey("");
    }
}

/**
 * 해당 유저가 서버에 존재하지 않을 때 => ERR_NOSUCHNICK_401
 * 해당 유저가 채널에 존재하지 않을 때 => 무시
 * 인자가 +o && 해당 채널에서 op가 아닐 때
 * 인자가 -o && 해당 채널에서 op일 때
 */
static void modeO(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &nick)
{
    if (mode_flag && nick == "")
    {
        client.addSendMsg(Response::ERR_SPECIFYPARAMETER_696(server, client, *channel, "op"));
        return;
    }
    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }
    Client *find_client = server.findClient(nick);
    if (find_client == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, nick));
        return;
    }
    if (!channel->isMember(*find_client))
    {
        return;
    }
    if (mode_flag && !channel->checkOpNick(nick))
    {
        channel->addOperator(nick);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName() + " +o",
                               find_client->getNick());
    }
    if (!mode_flag && channel->checkOpNick(nick))
    {
        channel->popOperator(nick);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName() + " -o",
                               find_client->getNick());
    }
}

/**
 * 인자가 +l
 * 인자가 -l && 해당 채널이 limit 모드일 때
 */
static void modeL(Server &server, Client &client, Channel *channel, bool mode_flag, const std::string &limit)
{
    if (mode_flag && limit == "")
    {
        client.addSendMsg(Response::ERR_SPECIFYPARAMETER_696(server, client, *channel, "limit"));
        return;
    }

    if (!channel->checkOp(client))
    {
        client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
        return;
    }

    int tmp = std::atoi(limit.c_str());
    if (mode_flag)
    {
        channel->setModeLimit(true);
        channel->setLimitCount(tmp);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName() + " +l", intToString(tmp));
    }
    if (!mode_flag && channel->getModeLimit())
    {
        channel->setModeLimit(false);
        channel->addSendMsgAll(server, client.getClientPrefix(), "MODE", channel->getName(), "-l");
    }
}
