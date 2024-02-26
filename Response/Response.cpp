#include "Response.hpp"

std::string Response::GENERATE(const std::string &prefix, const std::string &cmd, const std::string &args)
{
    return ":" + prefix + " " + cmd + " " + args + "\r\n";
}

// pong 응답
std::string Response::PONG(Server &server, const std::string &token)
{
    return GENERATE(server.getName(), "PONG", server.getName() + " :" + token);
}

std::string Response::RPL_WELCOME_001(Server &server, Client &client)
{
    return GENERATE(server.getName(), "001",
                    client.getNick() + " :Welcome to the FT-IRC Network " + client.getNick() + "!" + client.getUser());
}

// ERROR
std::string Response::ERR_NEEDMOREPARAMS_461(Server &server, Client &client, const std::string &command)
{
    return GENERATE(server.getName(), "461", client.getNick() + " " + command + " :Not enough parameters");
}

std::string Response::ERR_BADCHANMASK_476(Server &server, Client &client, const std::string &ch_name)
{
    return GENERATE(server.getName(), "476", client.getNick() + " " + ch_name + " :Bad Channel Mask");
}

// PASS USER
std::string Response::ERR_ALREADYREGISTERED_462(Server &server, Client &client)
{
    return GENERATE(server.getName(), "462", client.getNick() + " :You may not register");
}

std::string Response::RPL_USERHOST_302(Server &server, Client &client)
{
    return GENERATE(server.getName(), "302", client.getNick() + " :");
}

std::string Response::ERR_PASSWDMISMATCH_464(Server &server, Client &client)
{
    return GENERATE(server.getName(), "464", client.getNick() + " :Password incorrect");
}

// NICK
std::string Response::ERR_NONICKNAMEGIVEN_431(Server &server, Client &client)
{
    return GENERATE(server.getName(), "431", client.getNick() + " :No nickname given");
}

std::string Response::ERR_ERRONEUSNICKNAME_432(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "432", client.getNick() + " " + fail_nick + " :Erroneus nickname");
}

std::string Response::ERR_NICKNAMEINUSE_433(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "433", client.getNick() + " " + fail_nick + " :Nickname is already in use");
}

std::string Response::ERR_NICKCOLLISION_436(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "436", client.getNick() + " " + fail_nick + " :Nickname collision KILL");
}

// JOIN
std::string Response::RPL_TOPIC_332(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "332", client.getNick() + " " + channel.getName() + " :" + channel.getTopic());
}

std::string Response::ERR_INVITEONLYCHAN_473(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "473",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (invite only)");
}

std::string Response::ERR_BADCHANNELKEY_475(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "475",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (incorrect channel key)");
}

std::string Response::ERR_NOSUCHCHANNEL_403(Server &server, Client &client, const std::string &ch_name)
{
    return GENERATE(server.getName(), "403", client.getNick() + " " + ch_name + " :No such channel");
}

std::string Response::ERR_TOOMANYCHANNELS_405(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "405",
                    client.getNick() + " " + channel.getName() + " :You have joined too many channels");
}

std::string Response::ERR_CHANNEL_IS_FULL_471(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "471",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (channel is full)");
}

std::string Response::RPL_NAMREPLY_353(Server &server, Client &client, Channel &channel)
{
    std::string list = client.getNick() + " = " + channel.getName() + " :";
    for (size_t i = 0; i < channel.getNormals().size(); ++i)
    {
        if (channel.getNormals()[i] != channel.getNormals()[0])
        {
            list += " ";
        }
        if (channel.checkOp(*(channel.getNormals()[i])))
        {
            list += "@";
        }
        list += channel.getNormals()[i]->getNick();
    }
    return GENERATE(server.getName(), "353", list);
}

std::string Response::RPL_ENDOFNAMES_366(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "366",
                    client.getNick() + " " + channel.getName() + " :" + timeToString(channel.getCreated()));
}

// PART
std::string Response::ERR_NOTONCHANNEL_442(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "442",
                    client.getNick() + " " + channel.getName() + " :You're not on that channel");
}

// MODE
std::string Response::RPL_CHANNELMODEIS_324(Server &server, Client &client, Channel &channel)
{
    std::string modes = "";
    if (channel.getModeInvite())
    {
        modes += "i";
    }
    if (channel.getModeKey())
    {
        modes += "k";
    }
    if (channel.getModeLimit())
    {
        modes += "l";
    }
    if (channel.getModeTopic())
    {
        modes += "t";
    }
    if (channel.getKey() == "")
    {
        return GENERATE(server.getName(), "324", client.getNick() + " " + channel.getName() + " :" + modes);
    }
    return GENERATE(server.getName(), "324",
                    client.getNick() + " " + channel.getName() + " " + modes + " :" + channel.getKey());
}

std::string Response::RPL_CREATIONTIME_329(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "329",
                    client.getNick() + " " + channel.getName() + " :" + std::to_string(channel.getCreated()));
}

std::string Response::RPL_BANLIST_367(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "367", client.getNick() + " " + channel.getName());
}

std::string Response::RPL_ENDOFBANLIST_368(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "368", client.getNick() + " " + channel.getName() + " :End of channel ban list");
}

std::string Response::ERR_NOSUCHNICK_401(Server &server, Client &client, const std::string &no_nick)
{
    return GENERATE(server.getName(), "401", client.getNick() + " " + no_nick + " :No such nick");
}

std::string Response::ERR_KEYSET_467(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "467",
                    client.getNick() + " " + channel.getName() + " :This channel's key has already been set");
}

std::string Response::ERR_UNKNOWNMODE_472(Server &server, Client &client, const std::string &mode)
{
    return GENERATE(server.getName(), "472", client.getNick() + " " + mode + " :is unknown mode char to me");
}

std::string Response::ERR_CHANOPRIVSNEEDED_482(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "482",
                    client.getNick() + " " + channel.getName() + " :You're not channel operator");
}

std::string Response::ERR_UMODEUNKNWONFLAG_501(Server &server, Client &client, const std::string &mode)
{
    return GENERATE(server.getName(), "501", client.getNick() + mode + " :is an unknown mode character");
}

std::string Response::ERR_USERSDONTMATCH_502(Server &server, Client &client)
{
    return GENERATE(server.getName(), "502", client.getNick() + " :Can't view modes for other users");
}

std::string Response::ERR_SPECIFYPARAMETER_696(Server &server, Client &client, Channel &channel,
                                               const std::string &mode)
{
    return GENERATE(server.getName(), "696",
                    client.getNick() + " " + channel.getName() + " " + mode[0] +
                        " * :You must specify a parameter for the " + mode + " mode.");
}

// TOPIC
std::string Response::RPL_NOTOPIC_331(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "331", client.getNick() + " " + channel.getName() + " :No topic is set");
}

// LIST
std::string Response::RPL_LISTSTART_321(Server &server, Client &client)
{
    return GENERATE(server.getName(), "321", client.getNick() + " Channel :Users  Name");
}

std::string Response::RPL_LIST_322(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "322",
                    client.getNick() + " " + channel.getName() + " " + intToString(channel.getUserCount()) + " :" +
                        channel.getTopic());
}

std::string Response::RPL_LISTEND_323(Server &server, Client &client)
{
    return GENERATE(server.getName(), "323", client.getNick() + " :End of /LIST");
}

// INVITE
std::string Response::RPL_INVITING_341(Server &server, Client &client, Client &invite, Channel &channel)
{
    return GENERATE(server.getName(), "341", client.getNick() + " " + invite.getNick() + " " + channel.getName());
}

std::string Response::ERR_USERONCHANNEL_443(Server &server, Client &client, Client &invite, Channel &channel)
{
    return GENERATE(server.getName(), "443",
                    client.getNick() + " " + invite.getNick() + " " + channel.getName() + " :is already on channel");
}

// PRIVMSG
std::string Response::ERR_NORECIPIENT_411(Server &server, Client &client)
{
    return GENERATE(server.getName(), "411", client.getNick() + " :No recipient given");
}

std::string Response::ERR_NOTEXTTOSEND_412(Server &server, Client &client)
{
    return GENERATE(server.getName(), "412", client.getNick() + " :No text to send");
}

std::string Response::ERR_CANNOTSENDTOCHAN_404(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "404", client.getNick() + " " + channel.getName() + " :Cannot send to channel");
}

std::string Response::ERR_TOOMANYTARGETS_407(Server &server, Client &client)
{
    return GENERATE(server.getName(), "407", client.getNick() + " :Too many targets");
}

// WHO
std::string Response::RPL_WHOREPLY_352(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "352",
                    client.getNick() + " " + channel.getName() + " " + client.getNick() + " " + server.getName() + " " +
                        client.getNick() + " H@ :0 " + client.getNick());
}

std::string Response::RPL_ENDOFWHO_315(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "315", client.getNick() + " " + channel.getName() + " :End of /WHO list");
}

// MODE user +i
std::string Response::RPL_UMODEIS_221(Client &client)
{
    return GENERATE(client.getClientPrefix(), "MODE", client.getNick() + " :+");
}
