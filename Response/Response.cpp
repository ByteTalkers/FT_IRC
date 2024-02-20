#include "Response.hpp"

std::string Response::generateResponse(const std::string &prefix, const std::string &command, const std::string &args)
{
    return ":" + prefix + " " + command + " " + args + "\r\n";
}

// pong 응답
std::string Response::pongResponse(const std::string &se_name, const std::string &token)
{
    return generateResponse(se_name, "PONG", se_name + " :" + token);
}


std::string Response::rplWelcome_001(const std::string &se_name, const std::string &nick, const std::string &user)
{
    return generateResponse(se_name, "001", nick + " :Welcome to the FT-IRC Network " + nick + "!" + user);
}

// ERROR
std::string Response::errNeedMoreParams_461(const std::string &se_name, const std::string &nick, const std::string &command)
{
    return generateResponse(se_name, "461", nick + " " + command + " :Not enough parameters");
}

// PASS USER
std::string Response::errAlreadyRegistered_462(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "461", nick + " :You may not register");
}

// NICK
std::string Response::errNoNicknameGiven_431(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "431", nick + " :No nickname given");
}

std::string Response::errErroneusNickname_432(const std::string &se_name, const std::string &nick, const std::string &fail_nick)
{
    return generateResponse(se_name, "432", nick + " " + fail_nick + " :Erroneus nickname");
}

std::string Response::errNickNameInUse_433(const std::string &se_name, const std::string &nick, const std::string &fail_nick)
{
    return generateResponse(se_name, "433", nick + " " + fail_nick + " :Nickname is already in use");
}

std::string Response::errNickCollision_436(const std::string &se_name, const std::string &nick, const std::string &fail_nick)
{
    return generateResponse(se_name, "436", nick + " " + fail_nick + " :Nickname collision KILL");
}

// JOIN
std::string Response::rplTopic_332(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_topic)
{
    return generateResponse(se_name, "332", nick + " " + ch_name + " :" + ch_topic);
}

std::string Response::errInviteOnlyChan_473(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "473", nick + " " + ch_name + " :Cannot join channel (invite only)");
}

std::string Response::errBadChannelKey_475(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "475", nick + " " + ch_name + " :Cannot join channel (incorrect channel key)");
}

std::string Response::errNoSuchChannel_403(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "403", nick + " " + ch_name + " :No such channel");
}

std::string Response::errTooManyChannels_405(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "405", nick + " " + ch_name + " :You have joined too many channels");
}

// PART
std::string Response::errNoToNChannel_442(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "442", nick + " " + ch_name + " :You're not on that channel");
}

// MODE
std::string Response::rplChannelModeIs_324(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_modes, const std::string &ch_pass)
{
    if (ch_pass == "") 
    {
        return generateResponse(se_name, "324", nick + " " + ch_name + " :" + ch_modes);
    }
    return generateResponse(se_name, "324", nick + " " + ch_name + " " + ch_modes + " :" + ch_pass);
}

std::string Response::rplBanList_367(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "367", nick + " " + ch_name);
}

std::string Response::rplEndofBanList_368(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "368", nick + " " + ch_name + " :End of channel ban list");
}

std::string Response::errNoSuchNick_401(const std::string &se_name, const std::string &nick, const std::string &no_nick)
{
    return generateResponse(se_name, "401", nick + " " + no_nick + " :No such nick");
}

std::string Response::errKeySet_467(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "467", nick + " " + ch_name + " :This channel's key has already been set");
}

std::string Response::errUnknownMode_472(const std::string &se_name, const std::string &nick, const std::string &mode)
{
    return generateResponse(se_name, "472", nick + " "  + mode + " :is unknown mode char to me");
}

std::string Response::errChanOPrivsNeeded_482(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "482", nick + " " + ch_name + " :You're not channel operator");
}

// TOPIC
std::string Response::rplNoTopic_331(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "331", nick + " " + ch_name + " :No topic is set");
}

// LIST
std::string Response::rplListStart_321(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "321", nick + " Channel :Users  Name");
}

std::string Response::rplList_322(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_cl_count, const std::string &ch_topic)
{
    return generateResponse(se_name, "322", nick + " " + ch_name + " " + ch_cl_count + " :" + ch_topic);
}

std::string Response::rplListEnd_323(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "323", nick + " :End of /LIST");
}

// INVITE
std::string Response::rplInviting_341(const std::string &se_name, const std::string &nick, const std::string &invited, const std::string &ch_name)
{
    return generateResponse(se_name, "341", nick + " " + invited + " " + ch_name);
}

std::string Response::errUserOnChannel_443(const std::string &se_name, const std::string &nick, const std::string &invited, const std::string &ch_name)
{
    return generateResponse(se_name, "443", nick + " " + invited + " " + ch_name + " :is already on channel");
}

// PRIVMSG
std::string Response::errNoRecipient_411(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "411", nick + " :No recipient given");
}

std::string Response::errNoTextToSend_412(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "412", nick + " :No text to send");
}

std::string Response::errCanNotSendToChan_404(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "404", nick + " " + ch_name + " :Cannot send to channel");
}

std::string Response::errTooManyTargets_407(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "407", nick + " :Too many targets");
}

// WHO
std::string Response::rplWhoReply_352(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &user)
{
    return generateResponse(se_name, "352", nick + " " + ch_name + " " + user + " " + se_name + " " + nick + " H@ :0 " + user);
}

std::string Response::rplEndOfWho_315(const std::string &se_name, const std::string &nick, const std::string &ch_name)
{
    return generateResponse(se_name, "315", nick + " " + ch_name + " :End of /WHO list");
}

// PING PONG
std::string Response::errNoOrigin_409(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "409", nick + " :No origin specified");
}

// MODE user +i
std::string Response::rplUmodeIs_221(const std::string &user, const std::string &nick)
{
    return generateResponse(user, "MODE", nick + " :+i");
}

// WHOIS
std::string Response::rplWhoIsUser_311(const std::string &se_name, const std::string &nick, const std::string &user)
{
// nick> <user> <host> * :<real_name>
    return generateResponse(se_name, "311", nick + nick + user + " * " + " :" + user);
}

std::string Response::rplWhoIsServer_312(const std::string &se_name, const std::string &nick)
{
	// <nick> <server> :<server_info>
    // irc.local :Local IRC Server
    return generateResponse(se_name, "312", nick + " " + se_name + " :Local FT_IRC");
}

std::string Response::rplEndofWhoIs_318(const std::string &se_name, const std::string &nick)
{
	// <nick> :<info>
    return generateResponse(se_name, "318", nick + " :End of /WHOIS list.");
}

// Channel
std::string Response::rplChannelMsg(const std::string &from, const std::string &cmd, const std::string &to, const std::string &msg)
{
    return generateResponse(":" + from, cmd, to + " :" + msg);
}