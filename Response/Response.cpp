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

// PASS 
std::string Response::errNeedMoreParams_461(const std::string &se_name, const std::string &nick, std::string &command)
{
    return generateResponse(se_name, "461", nick + " " + command + " :Not enough parameters");
}

std::string Response::errAlreadyRegistered_462(const std::string &se_name, const std::string &nick)
{
    return generateResponse(se_name, "461", nick + " :You may not register");
}

// USER
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
