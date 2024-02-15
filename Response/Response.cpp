#include "Response.hpp"


std::string Response::generateRpl(const std::string &command, const std::string &to, const std::string &args)
{
    return command + " " + to + " " + args + "\r\n";
}

std::string Response::generateMessage(const std::string &command, const std::string &args)
{
    return command + " " + args + "\r\n";
}

std::string Response::generateErr(std::string &prefix, std::string &command, std::string &args)
{
    return ":" + prefix + " " + command + " " + args + "\r\n";
}



// pong 응답
std::string Response::pongResponse(const std::string &se_name, const std::string &token)
{
    return generateMessage("PONG", se_name + " :" + token);
}


std::string Response::rplWelcome_001(const std::string &nick, const std::string &user)
{
    // :irc.local 001 hj :Welcome to the Localnet IRC Network hj!root@127.0.0.1
    // " 001 " + nickname + ":welcome message " + nickname + "!" + username + "@" + ip

    return generateRpl("001", nick, ":Welcome to the FT-IRC Network " + nick + "!" + user);
}

// PASS 
std::string Response::errNeedMoreParams_461(std::string &se_name, std::string &command, std::string &reason)
{
    //<command> :<reason>
    return 
}