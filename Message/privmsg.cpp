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

void Message::privmsgExecute(Server &server, Client &client, Command *cmd)
{
    // 파라미터 없음
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
        // 채널 유저들에게 보내주기
        case CHANNEL:
            channel = server.findChannel(channel_name);
            // 채널에 포함됐는지 확인 필요
            if (channel->isMember(client))
            {
                sendPrivmsgToChannel(server, client, cmd->getParams());
            }
            else
            {
                client.addSendMsg(Response::ERR_CANNOTSENDTOCHAN_404(server, client, *channel));
                client.setWriteTypes(MYSELF);
            }
            break;
        // 해당 클라이언트에게만
        case CLIENT:
            sendPrivmsgToClient(server, client, cmd->getParams());
            break;
        case NOCHANNEL:
            client.addSendMsg(
                Response::ERR_NOSUCHCHANNEL_403(server, client, *channel));
            client.setWriteTypes(MYSELF);
            break;
        case NOCLIENT:
            client.addSendMsg(Response::ERR_NOSUCHNICK_401(server, client, cmd->getParams()[0]));
            client.setWriteTypes(MYSELF);
            break;
        }
    }
}

static void sendPrivmsgToChannel(Server &server, Client &client, const std::vector<std::string> &params)
{
    Channel *receiver = server.findChannel(params[0]);
    std::string msg = "";
    for (std::size_t i = 1; i < params.size(); i++)
    {
        msg += params[i];
    }
    receiver->addSendMsgAll(server, client.getNick(), "PRIVMSG", msg);
}

static void sendPrivmsgToClient(Server &server, Client &client, const std::vector<std::string> &params)
{
    Client *receiver = server.findClient(params[0]);
    std::string msg = "";
    for (std::size_t i = 1; i < params.size(); i++)
    {
        msg += params[i];
    }
    client.setRecvFd(receiver->getsockfd());
    receiver->addSendMsg(Response::GENERATE(client.getNick(), "PRIVMSG", receiver->getNick() + " :"  + msg).c_str());
    server.enableWriteEvent(receiver->getsockfd());
}

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