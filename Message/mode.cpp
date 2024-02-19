#include "Message.hpp"

static bool checkModeModifier(const std::string &params, bool &is_plus);

static void checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<bool, std::string>> &mode,
                          std::vector<char> &unknowns);

void Message::modeExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 있는지
    if (cmd->getParamsCount() > 1)
    {
        std::string channel_name;
        channel_name = cmd->getParams()[0];
        
        Channel *channel = server.findChannel(channel_name);
        // 채널 체크
        if (channel != NULL)
        {
            bool is_plus;
            // +, - 체크
            if (checkModeModifier(cmd->getParams()[1], is_plus) == true)
            {
                std::map<char, std::pair<bool, std::string>> mode;
                std::vector<char> unknowns;
                mode['i'] = std::make_pair(false, "");
                mode['t'] = std::make_pair(false, "");
                mode['k'] = std::make_pair(false, "");
                mode['o'] = std::make_pair(false, "");
                mode['l'] = std::make_pair(false, "");

                // 존재하는 모드인지 확인 modechar확인
                checkModeChar(cmd->getParams(), mode, unknowns);

                if (mode.at('i').first == true)
                {
                    if (channel->checkOp(client) == true)
                    {
                        channel->setModeInvite(true);
                        // mode +i 응답
                    }
                    else
                    {
                        client.addSendMsg(Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
                if (mode.at('t').first == true)
                {
                    if (channel->checkOp(client) == true)
                    {
                        channel->setModeTopic(true);
                        // mode +t 응답
                    }
                    else
                    {
                        client.addSendMsg(Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
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
                        client.addSendMsg(Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
                if (mode.at('o').first == true)
                {
                    if (channel->checkOp(client) == true)
                    {
                        // 채널 오퍼레이터에 추가
                        // mode +o 응답
                    }
                    else
                    {
                        client.addSendMsg(Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
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
                        client.addSendMsg(Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name));
                    }
                }
            }
            // +, - 없으면 unknown 에러? 에러 종류 정하기
            else
            {
                client.addSendMsg(Response::errUnknownMode_472(server.getName(), client.getNick(), cmd->getParams()[0]));
            }
        }
        else
        {
            if (cmd->getParams()[0] == client.getNick())
            {
                client.addSendMsg(Response::rplUmodeIs_221(client.getUser(), client.getNick()));
            }
            else
            {
                client.addSendMsg(Response::errNoSuchChannel_403(server.getName(), client.getNick(), channel_name));
            }
        }
    }
    else
    {
        // 파라미터 없으면 needmore err
        client.addSendMsg(Response::errNeedMoreParams_461(server.getName(), client.getNick(), cmd->getCommand()));
    }
}

static bool checkModeModifier(const std::string &params, bool &is_plus)
{
    if (params[0] == '+' || params[0] == '-')
    {
        if (params[0] == '+')
        {
            is_plus = true;
        }
        return true;
    }
    return false;
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

// check itkol
static void checkModeChar(const std::vector<std::string> &params, std::map<char, std::pair<bool, std::string>> &mode,
                          std::vector<char> &unknowns)
{
    int idx;
    idx = 1;
    for (std::size_t i = 1; i < params[1].length(); i++)
    {
        // itkol일 때,
        if (checkitkol(params[1][i]) == true)
        {
            mode[params[1][i]].first = true;
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
}