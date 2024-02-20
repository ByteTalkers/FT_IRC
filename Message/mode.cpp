#include "Message.hpp"

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<bool, std::string> > &mode,
                          std::vector<char> &unknowns);
static void modeI(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode);
static void modeT(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode);
static void modeK(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode);
static void modeO(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode);
static void modeL(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode);
static void modeUnknowns(Server &server, Client &client, std::vector<char> &unknowns);

void Message::modeExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 없음
    if (cmd->getParamsCount() < 2)
    {
        client.addSendMsg(Response::errNeedMoreParams_461(server.getName(), client.getNick(), cmd->getCommand()));
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
            client.addSendMsg(Response::rplUmodeIs_221(client.getUser(), client.getNick()));
        }
        else
        {
            client.addSendMsg(Response::errNoSuchChannel_403(server.getName(), client.getNick(), channel_name));
        }
        client.setWriteTypes(MYSELF);
        return;
    }


    // 모드와 내용 key 담는 map
    std::map<char, std::pair<bool, std::string> > mode;
    mode['i'] = std::make_pair(false, "");
    mode['t'] = std::make_pair(false, "");
    mode['k'] = std::make_pair(false, "");
    mode['o'] = std::make_pair(false, "");
    mode['l'] = std::make_pair(false, "");

    std::vector<char> unknowns;
    // 존재하는 모드인지 확인 modechar확인
    checkModeChar(cmd->getParams(), mode, unknowns);

    if (!checkModeChar(cmd->getParams(), mode, unknowns))
    {
        // unknowns에 다 넣기
        modeUnknowns(server, client, unknowns);
        return;
    }
    modeUnknowns(server, client, unknowns);
    modeI();
    modeT();
    modeK();
    modeO();
    modeL();
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

static bool checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<bool, std::string> > &mode,
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
            mode[params[1][i]].first = add;
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
            Response::errUnknownMode_472(server.getName(), client.getNick(), std::string(1, *it)));
    }
    client.setWriteTypes(MYSELF);
}


static void modeI(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode)
{
    if (mode.at('i').first == true)
    {
        if (channel->checkOp(client) == true)
        {
            channel->setModeInvite(true);
            // mode +i 응답
        }
        else
        {
            client.addSendMsg(
                Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel->getName()));
        }
    }    
}

static void modeT(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode)
{
    if (mode.at('t').first == true)
    {
        if (channel->checkOp(client) == true)
        {
            channel->setModeTopic(true);
            // mode +t 응답
        }
        else
        {
            client.addSendMsg(
                Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
        }
    }

}

static void modeK(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode)
{
    if (mode.at('k').first == true)
    {
        if (channel->checkOp(client) == true)
        {
            channel->setModeKey(true);
            channel->setKey(mode.at('k').second);
            // mode +k 응답
        }
        else
        {
            client.addSendMsg(
                Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
        }
    }

}

static void modeO(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode)
{
    if (mode.at('o').first == true)
    {
        if (channel->checkOp(client) == true)
        {
            // 채널 오퍼레이터에 추가
            // mode +o 응답
        }
        else
        {
            client.addSendMsg(
                Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
        }
    }

}

static void modeL(Server &server, Client &client, Channel *channel, std::map<char, std::pair<bool, std::string> > &mode)
{
    if (mode.at('l').first == true)
    {
        if (channel->checkOp(client) == true)
        {
            channel->setModeLimit(true);
            channel->setLimitCount(std::atoi(mode.at('l').second.c_str()));
            // mode +l 응답
        }
        else
        {
            client.addSendMsg(
                Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
        }
    }
}