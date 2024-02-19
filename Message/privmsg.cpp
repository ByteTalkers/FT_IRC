#include "Message.hpp"

enum ePrivmsg 
{
    CHANNEL,
    CLIENT,
    NOCHANNEL,
    NOCLIENT,
};

static std::vector<std::string> &splitComma(const std::string &command);
static ePrivmsg validCheck(Server &server, const std::string &to);
static void sendPrivmsgToClient(Server &server, Client &client, const std::string &to);
static void sendPrivmsgToChannel(Server &server, Client &client, const std::string &to);

void Message::privmsgExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() > 1)
    {
        std::vector<std::string> to = splitComma(cmd->getCommand());
        std::vector<std::string>::iterator it;
        for (it = to.begin(); it < to.end(); it++)
        {
            ePrivmsg to = validCheck(server, cmd->getParams()[0]);

            switch (to)
            {
                case CHANNEL:
                    // 채널 유저들에게 보내주기
                    // 채널에 포함됐는지 확인 필요
                    sendPrivmsgToChannel(server, client, cmd->getParams()[0]);
                    break;
                case CLIENT:
                    sendPrivmsgToClient(server, client, cmd->getParams()[0]);
                    break;
                case NOCHANNEL:
                    client.addSendMsg(Response::errNoSuchChannel_403(server.getName(), client.getNick(), cmd->getParams()[0]));
                    break;
                case NOCLIENT:
                    client.addSendMsg(Response::errNoSuchNick_401(server.getName(), client.getNick(), cmd->getParams()[0]));
                    break;
            }

        }

    }
    // 파라미터 부족
    else
    {
        client.addSendMsg(Response::errNeedMoreParams_461(server.getName(), client.getNick(), cmd->getCommand()));
    }
}

static void sendPrivmsgToChannel(Server &server, Client &client, const std::string &to, std::vector<std::string> params)
{
    Channel *receiver = server.findChannel(to);
    std::string msg = "";
    for (std::size_t i = 0; i < params.size(); i++)
    {
        msg += params[i];
    }
    receiver->addSendMsgAll(client.getNick(), msg);
    client.setWriteTypes(EVERYONE);
}

static void sendPrivmsgToClient(Server &server, Client &client, const std::string &to, std::vector<std::string> params)
{
    Client *receiver = server.findClient(to);
    std::string msg = "";
    for (std::size_t i = 0; i < params.size(); i++)
    {
        msg += params[i];
    }
    receiver->addSendMsg(Response::rplPrivmsg(client.getNick(), to, msg));
    receiver->setWriteTypes(MYSELF);
}


static std::vector<std::string> &splitComma(const std::string &command)
{
    std::vector<std::string> ret;
    std::istringstream iss(command);
    std::string buffer;

    while (getline, buffer, ' ')
    {
        ret.push_back(buffer);
    }

    return ret;
}

static ePrivmsg validCheck(Server &server, const std::string &to)
{
    if (to[0] == '&' || to[0] == '#')
    {
        Channel *channel = server.findChannel(to);
        if (channel != NULL)
        {
            return CHANNEL;
        }
        else
        {
            return NOCHANNEL;
        }
    }
    else
    {
        Client *client = server.findClient(to);
        if (client != NULL)
        {
            return CLIENT;
        }
        else
        {
            return NOCLIENT;
        }
    }

}