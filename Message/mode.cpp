#include "Message.hpp"

enum eCheck
{
    FALSE,
    TRUE,
    NOTEXIST
};

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<eCheck, std::string> > &mode,
                          std::vector<char> &unknowns);
static void modeI(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode);
static void modeT(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode);
static void modeK(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode);
static void modeO(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode);
static void modeL(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode);
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
            client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, *channel));
        }
        client.setWriteTypes(MYSELF);
        return;
    }

    // 모드와 내용 key 담는 map
    std::map<char, std::pair<eCheck, std::string> > mode;
    mode['i'] = std::make_pair(NOTEXIST, "");
    mode['t'] = std::make_pair(NOTEXIST, "");
    mode['k'] = std::make_pair(NOTEXIST, "");
    mode['o'] = std::make_pair(NOTEXIST, "");
    mode['l'] = std::make_pair(NOTEXIST, "");

    std::vector<char> unknowns;
    // 존재하는 모드인지 확인 modechar확인
    checkModeChar(cmd->getParams(), mode, unknowns);

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
        std::map<char, std::pair<eCheck, std::string> >::iterator it;
        for (it = mode.begin(); it != mode.end(); it++)
        {
            if ((*it).first != NOTEXIST)
            {
            client.addSendMsg(
                    Response::ERR_CHANOPRIVSNEEDED_482(server, client, *channel));
            }
            client.setWriteTypes(MYSELF);
            return;
        }
    }
    client.setWriteTypes(MYSELF);

    modeI(server, client, channel, mode);
    modeT(server, client, channel, mode);
    modeK(server, client, channel, mode);
    modeO(server, client, channel, mode);
    modeL(server, client, channel, mode);
    // client.setWriteTypes(EVERYONE);
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

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<eCheck, std::string> > &mode,
                          std::vector<char> &unknowns)
{
    // +, -로 시작 체크
    if (params[1][0] != '+' && params[1][0] != '-')
    {
        return false;
    }

    std::size_t idx = 1;
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
            // +인지 -인지 확인 후 세팅
            if (add == true) 
            {
                mode[params[1][i]].first = TRUE;
            }
            else
            {
                mode[params[1][i]].first = FALSE;
            }

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
            unknowns.push_back(params[1][i]);
        }
    }

    // 남은 파라미터들은 unknowns로
    while(idx < params.size())
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
        client.addSendMsg(
            Response::ERR_UNKNOWNMODE_472(server, client, std::string(1, *it)));
    }
}


static void modeI(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode)
{
    if (mode.at('i').first == NOTEXIST)
    {
        return;
    }

    // 인자는 +i, 해당 채널 i 세팅 아님
    if (mode.at('i').first == TRUE && !channel->getModeInvite())
    {
        channel->setModeInvite(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-i");
    }
    // 인자는 -i, 해당 채널 i 세팅인
    if (mode.at('i').first == FALSE && channel->getModeInvite())
    {
        channel->setModeInvite(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+i");
    }    
}

static void modeT(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode)
{
    if (mode.at('t').first == NOTEXIST)
    {
        return;
    }

    if (mode.at('t').first == TRUE && !channel->getModeTopic())
    {
        channel->setModeTopic(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-t");
    }
    if (mode.at('t').first == FALSE && channel->getModeTopic())
    {
        channel->setModeTopic(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+t");
    }    
}

static void modeK(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode)
{
    if (mode.at('k').first == NOTEXIST)
    {
        return;
    }

    if (mode.at('k').first == TRUE && !channel->getModeKey())
    {
        channel->setModeKey(true);
        channel->setKey(mode.at('k').second);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-k");
    }
    if (mode.at('k').first == FALSE && channel->getModeKey())
    {
        channel->setModeKey(false);
        channel->setKey("");
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+k");
    }
}

static void modeO(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode)
{
    if (mode.at('o').first == NOTEXIST)
    {
        return;
    }

    // 클라이언트 이름으로 채널에 있는지 확인하는 함수 추가
    if (mode.at('o').first == TRUE)
    {
        channel->setModeTopic(true);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-o");
    }
    if (mode.at('t').first == FALSE)
    {
        channel->setModeTopic(false);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+o");
    }
}

static void modeL(Server &server, Client &client, Channel *channel, std::map<char, std::pair<eCheck, std::string> > &mode)
{
    if (mode.at('l').first == NOTEXIST)
    {
        return;
    }

    if (mode.at('l').first == TRUE && !channel->getModeLimit())
    {
        channel->setModeLimit(true);
        channel->setLimitCount(std::atoi(mode.at('l').second.c_str()));
        channel->addSendMsgAll(server, client.getNick(), "MODE", "-l");
    }
    if (mode.at('l').first == FALSE && channel->getModeLimit())
    {
        channel->setModeLimit(false);
        channel->setLimitCount(0);
        channel->addSendMsgAll(server, client.getNick(), "MODE", "+l");
    }
}