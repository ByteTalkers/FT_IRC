#include "Response.hpp"

/**
 * @brief Generates a response string with the given prefix, command, and arguments.
 *
 * @param prefix The prefix of the response.
 * @param cmd The command of the response.
 * @param args The arguments of the response.
 * @return The generated response string.
 */
std::string Response::GENERATE(const std::string &prefix, const std::string &cmd, const std::string &args)
{
    return ":" + prefix + " " + cmd + " " + args + "\r\n";
}

// pong 응답
/**
 * @brief Represents a sequence of characters.
 *
 * The std::string class is a container that holds and manipulates a sequence of characters.
 * It provides various member functions to perform operations on strings, such as concatenation,
 * substring extraction, and searching.
 *
 * @note This class is part of the C++ Standard Library.
 *
 * @see https://en.cppreference.com/w/cpp/string/basic_string
 */
std::string Response::PONG(Server &server, const std::string &token)
{
    return GENERATE(server.getName(), "PONG", server.getName() + " :" + token);
}

/**
 * @brief Returns a string representing the welcome message for the client.
 *
 * This function generates a welcome message for the client by combining the server name, client nickname,
 * and client user information.
 *
 * @param server The server object.
 * @param client The client object.
 * @return A string representing the welcome message.
 */
std::string Response::RPL_WELCOME_001(Server &server, Client &client)
{
    return GENERATE(server.getName(), "001",
                    client.getNick() + " :Welcome to the FT-IRC Network " + client.getNick() + "!" + client.getUser());
}

// ERROR
/**
 * @brief Constructs a std::string object.
 *
 * @param server The server object.
 * @param client The client object.
 * @param command The command string.
 * @return A string representing the error message.
 */
std::string Response::ERR_NEEDMOREPARAMS_461(Server &server, Client &client, const std::string &command)
{
    return GENERATE(server.getName(), "461", client.getNick() + " " + command + " :Not enough parameters");
}

/**
 * @brief Constructs a std::string object.
 *
 * @param server The server object.
 * @param client The client object.
 * @param ch_name The channel name.
 * @return The generated string containing server name, error code, client nickname, channel name, and error message.
 */
std::string Response::ERR_BADCHANMASK_476(Server &server, Client &client, const std::string &ch_name)
{
    return GENERATE(server.getName(), "476", client.getNick() + " " + ch_name + " :Bad Channel Mask");
}

// PASS USER
/**
 * @brief Returns an error response message for the ERR_ALREADYREGISTERED_462 error code.
 *
 * This function generates an error response message for the ERR_ALREADYREGISTERED_462 error code,
 * which indicates that the client is already registered on the server and cannot register again.
 *
 * @param server The server object.
 * @param client The client object.
 * @return The error response message.
 */
std::string Response::ERR_ALREADYREGISTERED_462(Server &server, Client &client)
{
    return GENERATE(server.getName(), "462", client.getNick() + " :You may not register");
}

/**
 * @brief Returns a string representation of the RPL_USERHOST_302 response.
 *
 * This function generates a string that represents the RPL_USERHOST_302 response
 * for a given server and client. The generated string includes the server name,
 * the response code "302", and the client's nickname followed by a colon.
 *
 * @param server The server object.
 * @param client The client object.
 * @return std::string The generated response string.
 */
std::string Response::RPL_USERHOST_302(Server &server, Client &client)
{
    return GENERATE(server.getName(), "302", client.getNick() + " :");
}

/**
 * @brief Constructs a std::string object.
 *
 * This function constructs a std::string object and returns it.
 *
 * @param server The server object.
 * @param client The client object.
 * @return The constructed std::string object.
 */
std::string Response::ERR_PASSWDMISMATCH_464(Server &server, Client &client)
{
    return GENERATE(server.getName(), "464", client.getNick() + " :Password incorrect");
}

// NICK
/**
 * @brief Returns an error response string for ERR_NONICKNAMEGIVEN_431.
 *
 * This function generates an error response string for the ERR_NONICKNAMEGIVEN_431
 * error code. The response string includes the server name, error code, and a message
 * indicating that no nickname was given.
 *
 * @param server The server object.
 * @param client The client object.
 * @return The error response string.
 */
std::string Response::ERR_NONICKNAMEGIVEN_431(Server &server, Client &client)
{
    return GENERATE(server.getName(), "431", client.getNick() + " :No nickname given");
}

/**
 * @brief Constructs a std::string object.
 *
 * @param str The initial value of the string.
 */
std::string Response::ERR_ERRONEUSNICKNAME_432(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "432", client.getNick() + " " + fail_nick + " :Erroneus nickname");
}

/**
 * @brief Generates an error response message for the ERR_NICKNAMEINUSE_433 command.
 *
 * This function generates an error response message for the ERR_NICKNAMEINUSE_433 command,
 * which indicates that the requested nickname is already in use.
 *
 * @param server The server object.
 * @param client The client object.
 * @param fail_nick The nickname that failed to be used.
 * @return The generated error response message.
 */
std::string Response::ERR_NICKNAMEINUSE_433(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "433", client.getNick() + " " + fail_nick + " :Nickname is already in use");
}

/**
 * @brief Generates an error response for the ERR_NICKCOLLISION_436 command.
 *
 * This function generates a string response for the ERR_NICKCOLLISION_436 command, which is used to indicate a nickname
 * collision.
 *
 * @param server The server object.
 * @param client The client object.
 * @param fail_nick The nickname that caused the collision.
 * @return The generated response string.
 */
std::string Response::ERR_NICKCOLLISION_436(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "436", client.getNick() + " " + fail_nick + " :Nickname collision KILL");
}

// JOIN
/**
 * @brief Returns a string representing the response for RPL_TOPIC_332.
 *
 * This function generates a string response for the RPL_TOPIC_332 command, which is used to send the topic of a channel
 * to a client. The response string is generated by concatenating the server name, the numeric code "332", the client's
 * nickname, the channel name, and the channel's topic.
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @return std::string The response string for RPL_TOPIC_332.
 */
std::string Response::RPL_TOPIC_332(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "332", client.getNick() + " " + channel.getName() + " :" + channel.getTopic());
}

/**
 * @brief Represents a sequence of characters.
 *
 * The std::string class is a container that holds a sequence of characters. It provides various member functions
 * for manipulating and accessing the characters in the string.
 */
std::string Response::ERR_INVITEONLYCHAN_473(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "473",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (invite only)");
}

/**
 * @brief Constructs a std::string object.
 *
 * This function constructs a std::string object with the specified parameters.
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @return A std::string object representing the error message.
 */
std::string Response::ERR_BADCHANNELKEY_475(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "475",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (incorrect channel key)");
}

/**
 * @brief Constructs a std::string object.
 *
 * @param server The server object.
 * @param client The client object.
 * @param ch_name The name of the channel.
 * @return A string representing the error message.
 */
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

/**
 * @brief Returns a string representing the response for RPL_CREATIONTIME_329.
 *
 * This function generates a string response for the RPL_CREATIONTIME_329 command, which provides information about the
 * creation time of a channel. The response format is "<server name> 329 <client nickname> <channel name> :<creation
 * time>".
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @return A string representing the response for RPL_CREATIONTIME_329.
 */
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

/**
 * @brief Constructs a std::string object.
 *
 * @param no_nick The nickname that does not exist.
 * @return A std::string object representing the error message.
 */
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
    return GENERATE(server.getName(), "501", client.getNick() + " " + mode + " :is an unknown mode character");
}

std::string Response::ERR_USERSDONTMATCH_502(Server &server, Client &client)
{
    return GENERATE(server.getName(), "502", client.getNick() + " :Can't view modes for other users");
}

/**
 * @brief Returns an error response message with a specific parameter for the mode.
 *
 * This function generates an error response message with the specified parameter for the mode.
 * The message format is as follows:
 * "<server name> 696 <client nickname> <channel name> <mode[0]> * :You must specify a parameter for the <mode> mode."
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @param mode The mode parameter.
 * @return The error response message.
 */
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
/**
 * @brief Returns an error response string for the ERR_NORECIPIENT_411 error code.
 *
 * This function generates an error response string for the ERR_NORECIPIENT_411 error code,
 * which indicates that no recipient was given.
 *
 * @param server The server object.
 * @param client The client object.
 * @return The error response string.
 */
std::string Response::ERR_NORECIPIENT_411(Server &server, Client &client)
{
    return GENERATE(server.getName(), "411", client.getNick() + " :No recipient given");
}

std::string Response::ERR_NOTEXTTOSEND_412(Server &server, Client &client)
{
    return GENERATE(server.getName(), "412", client.getNick() + " :No text to send");
}

/**
 * @brief Returns an error response string for the ERR_CANNOTSENDTOCHAN_404 error code.
 *
 * This function generates an error response string for the ERR_CANNOTSENDTOCHAN_404 error code,
 * which indicates that the client cannot send a message to the specified channel.
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @return The error response string.
 */
std::string Response::ERR_CANNOTSENDTOCHAN_404(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "404", client.getNick() + " " + channel.getName() + " :Cannot send to channel");
}

std::string Response::ERR_TOOMANYTARGETS_407(Server &server, Client &client)
{
    return GENERATE(server.getName(), "407", client.getNick() + " :Too many targets");
}

// WHO
/**
 * @brief Returns a string representing the response for RPL_WHOREPLY_352.
 *
 * This function generates a string response for the RPL_WHOREPLY_352 command in the IRC protocol.
 * The response includes the server name, client nickname, channel name, and other relevant information.
 *
 * @param server The server object.
 * @param client The client object.
 * @param channel The channel object.
 * @return std::string The generated response string.
 */
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
