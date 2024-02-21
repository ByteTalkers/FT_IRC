#include "Message.hpp"

enum eCheck
{
    FALSE,
    TRUE,
    NOTEXIST
};

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<eCheck, std::string>> &mode,
                          std::vector<char> &unknowns);
static void modeI(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode);
static void modeT(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode);
static void modeK(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode);
static void modeO(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode);
static void modeL(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode);
static void modeUnknowns(Server &server, Client &client, std::vector<char> &unknowns);

void Message::modeExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 없음
    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }

    std::string channel_name;
    channel_name = cmd->getParams()[0];
    Channel *channel = server.findChannel(channel_name);
    // 채널 체크
    if (channel == NULL)
    {
        if (cmd->getParams()[0] == client.getNick())
        {
            client.addSendMsg(Response::RPL_UMODEIS_221(client));
        }
        else
        {
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getParams()[0]));
        }
        client.setWriteTypes(MYSELF);
        return;
    }

    // 모드와 내용 key 담는 map
    std::map<char, std::pair<eCheck, std::string>> mode;
    mode['i'] = std::make_pair(NOTEXIST, "");
    mode['t'] = std::make_pair(NOTEXIST, "");
    mode['k'] = std::make_pair(NOTEXIST, "");
    mode['o'] = std::make_pair(NOTEXIST, "");
    mode['l'] = std::make_pair(NOTEXIST, "");

    std::vector<char> unknowns;
    // 존재하는 모드인지 확인 modechar확인

    if (!checkModeChar(cmd->getParams(), mode, unknowns))
    {
        // unknowns에 다 넣기
        modeUnknowns(server, client, unknowns);
        client.setWriteTypes(MYSELF);
        return;
    }

    modeUnknowns(server, client, unknowns);
    // 방장 아닌 경우
    if (!channel->checkOp(client))
    {
        std::map<char, std::pair<eCheck, std::string>>::iterator it;
        for (it = mode.begin(); it != mode.end(); it++)
        {
            if ((*it).first != NOTEXIST)
            {
                client.addSendMsg(Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
            }
            client.setWriteTypes(MYSELF);
            return;
        }
    }
    client.setWriteTypes(MYSELF);

    modeI(server, client, channel, mode.at('i'));
    modeT(server, client, channel, mode.at('t'));
    modeK(server, client, channel, mode.at('k'));
    modeO(server, client, channel, mode.at('o'));
    modeL(server, client, channel, mode.at('l'));
}

static bool checkitkol(char c)
{
    const char check[5] = {'i', 't', 'k', 'o', 'l'};

    for (int i = 0; i < 5; i++)
    {
        if (c == check[i])
        {
            return true;
        }
    }
    return false;
}

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<eCheck, std::string>> &mode,
                          std::vector<char> &unknowns)
{
    // +, -로 시작 체크
    if (params[1][0] != '+' && params[1][0] != '-')
    {
        return false;
    }

    std::size_t idx = 2;
    bool add = false;
    for (std::size_t i = 0; i < params[1].length(); i++)
    {
        if (params[1][i] == '+' || params[1][i] == '-')
        {
            if (params[1][i] == '+')
            {
                add = true;
            }
            else
            {
                add = false;
            }
            continue;
        }
        // itkol일 때,
        if (checkitkol(params[1][i]) == true)
        {
            std::cout << "this is " << params[1][i] << std::endl;
            // +인지 -인지 확인 후 세팅
            if (add == true)
            {
                mode[params[1][i]].first = TRUE;
            }
            else
            {
                mode[params[1][i]].first = FALSE;
            }
            // kol 인 경우 파라미터 채워주기
            if (params[1][i] == 'k' || params[1][i] == 'o' || params[1][i] == 'l')
            {
                if (idx < params.size())
                {
                    mode[params[1][i]].second = params[idx];
                    idx++;
                }
            }
        }
        else
        {
            if (params[1][i] != '+' || params[1][i] != '-')
            {
                unknowns.push_back(params[1][i]);
            }
        }
    }

    // 남은 파라미터들은 unknowns로
    while (idx < params.size())
    {
        for (std::size_t i = 0; i < params[idx].length(); i++)
        {
            unknowns.push_back(params[idx][i]);
        }
        idx++;
    }

    return true;
}

static void modeUnknowns(Server &server, Client &client, std::vector<char> &unknowns)
{
    std::vector<char>::iterator it;
    for (it = unknowns.begin(); it < unknowns.end(); it++)
    {
        client.addSendMsg(Response::ERR_UNKNOWNMODE_472(server, client, std::string(1, *it)));
    }
}

static void modeI(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode)
{
    // 인자는 +i, 해당 채널 i 세팅 아님
    if (mode.first == TRUE && !channel->getModeInvite())
    {
        channel->setModeInvite(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-i");
    }
    // 인자는 -i, 해당 채널 i 세팅인
    if (mode.first == FALSE && channel->getModeInvite())
    {
        channel->setModeInvite(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+i");
    }
}

static void modeT(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode)
{
    if (mode.first == TRUE && !channel->getModeTopic())
    {
        channel->setModeTopic(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-t");
    }
    if (mode.first == FALSE && channel->getModeTopic())
    {
        channel->setModeTopic(false);
        channel->setTopicExist(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+t");
    }
}

static void modeK(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode)
{
    if (mode.first == TRUE && !channel->getModeKey())
    {
        channel->setModeKey(true);
        channel->setKey(mode.second);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-k");
    }
    if (mode.first == FALSE && channel->getModeKey())
    {
        channel->setModeKey(false);
        channel->setKey("");
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+k");
    }
}

static void modeO(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode)
{

    if (!channel->isMemberNick(mode.second))
    {
        client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, mode.second));
        client.setWriteTypes(MYSELF);
    }
    if (mode.first == TRUE)
    {
        channel->setModeTopic(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-o");
    }
    if (mode.first == FALSE)
    {
        channel->setModeTopic(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+o");
    }
}

static void modeL(Server &server, Client &client, Channel *channel, std::pair<eCheck, std::string> &mode)
{
    if (mode.first == TRUE && !channel->getModeLimit())
    {
        channel->setModeLimit(true);
        channel->setLimitCount(std::atoi(mode.second.c_str()));
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-l");
    }
    if (mode.first == FALSE && channel->getModeLimit())
    {
        channel->setModeLimit(false);
        channel->setLimitCount(0);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+l");
    }
}