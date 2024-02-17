#include "Message.hpp"

static bool checkModeModifier(std::string &params, bool &is_plus);

static void checkModeChar(std::vector<std::string> &params, std::map<char, std::pair<bool, std::string>> &mode,
                          std::vector<char> &unknowns);

void Message::modeExecute(Server &server, Client &client)
{
    // 파라미터 있는지
    std::size_t params_count;
    params_count = this->m_params.size();
    if (params_count > 1)
    {
        std::string channel_name;
        channel_name = this->m_params[0];
        // 채널 체크
        if (server.channelExist(channel_name) == true)
        {
            bool is_plus;
            // +, - 체크
            if (checkModeModifier(this->m_params[1], is_plus) == true)
            {
                Channel *channel;
                channel = server.getChannels().at(channel_name);

                std::map<char, std::pair<bool, std::string>> mode;
                std::vector<char> unknowns;
                mode['i'] = std::make_pair(false, "");
                mode['t'] = std::make_pair(false, "");
                mode['k'] = std::make_pair(false, "");
                mode['o'] = std::make_pair(false, "");
                mode['l'] = std::make_pair(false, "");

                // 존재하는 모드인지 확인 modechar확인
                checkModeChar(this->m_params, mode, unknowns);

                if (mode.at('i').first == true)
                {
                    if (channel->checkOp(&client) == true)
                    {
                        channel->setModeInvite(true);
                        // mode +i 응답
                    }
                    else
                    {
                        Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                    }
                }
                if (mode.at('t').first == true)
                {
                    if (channel->checkOp(&client) == true)
                    {
                        channel->setModeTopic(true);
                        // mode +t 응답
                    }
                    else
                    {
                        Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                    }
                }
                if (mode.at('k').first == true)
                {
                    if (channel->checkOp(&client) == true)
                    {
                        channel->setModeKey(true);
                        channel->setKey(mode.at('k').second);
                        // mode +k 응답
                    }
                    else
                    {
                        Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                    }
                }
                if (mode.at('o').first == true)
                {
                    if (channel->checkOp(&client) == true)
                    {
                        // 채널 오퍼레이터에 추가
                        // mode +o 응답
                    }
                    else
                    {
                        Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                    }
                }
                if (mode.at('l').first == true)
                {
                    if (channel->checkOp(&client) == true)
                    {
                        channel->setModeLimit(true);
                        channel->setLimitCount(std::atoi(mode.at('l').second.c_str()));
                        // mode +l 응답
                    }
                    else
                    {
                        Response::errChanOPrivsNeeded_482(server.getName(), client.getNick(), channel_name);
                    }
                }
            }
            // +, - 없으면 unknown 에러? 에러 종류 정하기
            else
            {
                Response::errUnknownMode_472(server.getName(), client.getNick(), this->m_params[0]);
            }
        }
        else
        {
            Response::errNoSuchChannel_403(server.getName(), client.getNick(), channel_name);
        }
    }
    // 존재하는 모드면 나머지 파라미터 확인
    // 파라미터 없으면 neemore err
    // 파라미터 있는 애면 op인지도 확인
    // i
    // t
    // k
    // o
    // l
    // op 아니면 privil 에러
    // 존재하지 않는 모드면 unknown err
    else
    {
        // 파라미터 없으면 needmore err
        Response::errNeedMoreParams_461(server.getName(), client.getNick(), m_command);
    }
}

static bool checkModeModifier(std::string &params, bool &is_plus)
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
static void checkModeChar(std::vector<std::string> &params, std::map<char, std::pair<bool, std::string>> &mode,
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