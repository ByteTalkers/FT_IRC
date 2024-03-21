#include "Response.hpp"

/**
 * @brief std::string 타입에 대한 문서화 주석입니다.
 *
 * std::string은 문자열을 저장하는 클래스입니다.
 * C++ 표준 라이브러리에서 제공되며, 문자열을 다루는 다양한 기능을 제공합니다.
 *
 * @param prefix 문자열의 접두사(prefix)로 사용될 값
 * @param cmd 문자열의 명령어(cmd)로 사용될 값
 * @param args 문자열의 인수(args)로 사용될 값
 * @return 문자열의 접두사, 명령어, 인수를 조합한 결과 문자열
 */
std::string Response::GENERATE(const std::string &prefix, const std::string &cmd, const std::string &args)
{
    return ":" + prefix + " " + cmd + " " + args + "\r\n";
}

// pong 응답
/**
 * @brief Response 클래스의 PONG 함수입니다.
 *
 * 이 함수는 서버에게 PONG 메시지를 보내는 역할을 합니다.
 *
 * @param server 서버 객체에 대한 참조입니다.
 * @param token PONG 메시지에 포함될 토큰입니다.
 * @return PONG 메시지를 성공적으로 보냈을 경우 true를 반환합니다.
 */
std::string Response::PONG(Server &server, const std::string &token)
{
    return GENERATE(server.getName(), "PONG", server.getName() + " :" + token);
}

/**
 * @brief RPL_WELCOME_001에 대한 설명입니다.
 *
 * 이 함수는 서버와 클라이언트의 연결이 성공적으로 이루어졌을 때 보내는 환영 메시지를 처리합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @return std::string 환영 메시지
 */
std::string Response::RPL_WELCOME_001(Server &server, Client &client)
{
    return GENERATE(server.getName(), "001",
                    client.getNick() + " :Welcome to the FT-IRC Network " + client.getNick() + "!" + client.getUser());
}

// ERROR
/**
 * @brief ERR_NEEDMOREPARAMS_461에 대한 설명입니다.
 *
 * ERR_NEEDMOREPARAMS_461는 서버와 클라이언트 간의 통신에서 발생하는 오류 코드입니다.
 * 이 오류는 클라이언트가 명령에 필요한 추가 매개변수를 제공하지 않았을 때 발생합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param command 명령 문자열
 * @return std::string 오류 메시지
 */
std::string Response::ERR_NEEDMOREPARAMS_461(Server &server, Client &client, const std::string &command)
{
    return GENERATE(server.getName(), "461", client.getNick() + " " + command + " :Not enough parameters");
}

/**
 * @brief Response::ERR_BADCHANMASK_476 함수는 채널 마스크가 잘못된 경우에 대한 응답을 생성합니다.
 *
 * @param server 서버 객체에 대한 참조
 * @param client 클라이언트 객체에 대한 참조
 * @param ch_name 채널 이름에 대한 문자열 참조
 * @return 생성된 응답 문자열
 */
std::string Response::ERR_BADCHANMASK_476(Server &server, Client &client, const std::string &ch_name)
{
    return GENERATE(server.getName(), "476", client.getNick() + " " + ch_name + " :Bad Channel Mask");
}

// PASS USER
/**
 * @brief Response::ERR_ALREADYREGISTERED_462 함수는 이미 등록된 클라이언트에 대한 오류 응답을 생성합니다.
 *
 * @param server 서버 객체에 대한 참조
 * @param client 클라이언트 객체에 대한 참조
 * @return std::string 오류 응답 문자열
 */
std::string Response::ERR_ALREADYREGISTERED_462(Server &server, Client &client)
{
    return GENERATE(server.getName(), "462", client.getNick() + " :You may not register");
}

/**
 * @brief RPL_USERHOST_302 함수는 서버와 클라이언트 간의 통신에서 사용되는 응답 코드입니다.
 * 이 함수는 서버와 클라이언트의 사용자 호스트 정보를 반환합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @return 사용자 호스트 정보를 나타내는 문자열
 */
std::string Response::RPL_USERHOST_302(Server &server, Client &client)
{
    return GENERATE(server.getName(), "302", client.getNick() + " :");
}

/**
 * @brief Response::ERR_PASSWDMISMATCH_464 함수는 서버와 클라이언트 사이에서 비밀번호 불일치 오류를 처리하는 함수입니다.
 *
 * @param server 서버 객체에 대한 참조입니다.
 * @param client 클라이언트 객체에 대한 참조입니다.
 * @return std::string 오류 메시지를 나타내는 문자열입니다.
 */
std::string Response::ERR_PASSWDMISMATCH_464(Server &server, Client &client)
{
    return GENERATE(server.getName(), "464", client.getNick() + " :Password incorrect");
}

// NICK
/**
 * @brief ERR_NONICKNAMEGIVEN_431에 대한 설명입니다.
 *
 * 이 함수는 서버와 클라이언트 객체를 인자로 받아서 ERR_NONICKNAMEGIVEN_431 응답을 생성합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @return 생성된 응답 문자열
 */
std::string Response::ERR_NONICKNAMEGIVEN_431(Server &server, Client &client)
{
    return GENERATE(server.getName(), "431", client.getNick() + " :No nickname given");
}

/**
 * @brief Response::ERR_ERRONEUSNICKNAME_432 함수는 ERR_ERRONEUSNICKNAME_432 응답을 생성하는 메서드입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 잘못된 닉네임(fail_nick)을 매개변수로 받아서
 * ERR_ERRONEUSNICKNAME_432 응답을 생성합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param fail_nick 잘못된 닉네임
 * @return 생성된 응답 문자열
 */
std::string Response::ERR_ERRONEUSNICKNAME_432(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "432", client.getNick() + " " + fail_nick + " :Erroneus nickname");
}

/**
 * @brief ERR_NICKNAMEINUSE_433 함수는 서버와 클라이언트 간의 통신에서 사용되는 오류 응답 메시지를 생성하는 함수입니다.
 *
 * @param server 서버 객체에 대한 참조입니다.
 * @param client 클라이언트 객체에 대한 참조입니다.
 * @param fail_nick 사용할 수 없는 닉네임을 나타내는 문자열입니다.
 * @return 생성된 오류 응답 메시지를 나타내는 문자열입니다.
 */
std::string Response::ERR_NICKNAMEINUSE_433(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "433", client.getNick() + " " + fail_nick + " :Nickname is already in use");
}

/**
 * @brief ERR_NICKCOLLISION_436에 대한 설명입니다.
 *
 * ERR_NICKCOLLISION_436는 닉네임 충돌 오류를 나타내는 상수입니다.
 * 이 오류는 서버와 클라이언트 간에 닉네임이 충돌할 때 발생합니다.
 *
 * @param server 서버 객체입니다.
 * @param client 클라이언트 객체입니다.
 * @param fail_nick 충돌한 닉네임입니다.
 * @return std::string 오류 메시지를 반환합니다.
 */
std::string Response::ERR_NICKCOLLISION_436(Server &server, Client &client, const std::string &fail_nick)
{
    return GENERATE(server.getName(), "436", client.getNick() + " " + fail_nick + " :Nickname collision KILL");
}

// JOIN
/**
 * @brief Response::RPL_TOPIC_332 함수는 서버에서 클라이언트에게 채널의 주제를 전송하는 역할을 합니다.
 *
 * @param server 서버 객체에 대한 참조입니다.
 * @param client 클라이언트 객체에 대한 참조입니다.
 * @param channel 채널 객체에 대한 참조입니다.
 * @return std::string 채널의 주제를 나타내는 문자열입니다.
 */
std::string Response::RPL_TOPIC_332(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "332", client.getNick() + " " + channel.getName() + " :" + channel.getTopic());
}

/**
 * @brief Response::ERR_INVITEONLYCHAN_473 함수는 INVITE 명령어를 처리할 때,
 * 클라이언트가 초대받은 채널에 참여할 수 없는 경우에 대한 응답 메시지를 생성합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param channel 채널 객체
 * @return std::string 생성된 응답 메시지
 */
std::string Response::ERR_INVITEONLYCHAN_473(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "473",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (invite only)");
}

/**
 * @brief 채널 키가 잘못된 경우에 대한 응답 코드입니다.
 *
 * 이 응답 코드는 채널에 대한 키가 잘못된 경우에 사용됩니다.
 * 클라이언트는 올바른 키를 제공해야 채널에 참여할 수 있습니다.
 *
 * @see Response
 */
std::string Response::ERR_BADCHANNELKEY_475(Server &server, Client &client, Channel &channel)
{
    return GENERATE(server.getName(), "475",
                    client.getNick() + " " + channel.getName() + " :Cannot join channel (incorrect channel key)");
}

/**
 * @brief ERR_NOSUCHCHANNEL_403에 대한 설명입니다.
 *
 * ERR_NOSUCHCHANNEL_403는 채널이 존재하지 않을 때 발생하는 오류를 나타내는 상수입니다.
 * 이 오류는 서버와 클라이언트, 그리고 채널 이름을 매개변수로 받아 처리됩니다.
 *
 * @param server 서버 객체입니다.
 * @param client 클라이언트 객체입니다.
 * @param ch_name 채널 이름입니다.
 * @return std::string 오류 메시지를 반환합니다.
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
 * @brief Response::RPL_CREATIONTIME_329 함수는 서버에서 클라이언트에게 채널의 생성 시간을 알려주는 응답을 보내는
 * 함수입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param channel 채널 객체
 * @return std::string 응답 메시지
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
 * @brief ERR_NOSUCHNICK_401 함수는 서버와 클라이언트, 그리고 존재하지 않는 닉네임을 받아서 해당하는 오류 메시지를
 * 생성하여 반환합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param no_nick 존재하지 않는 닉네임
 * @return 생성된 오류 메시지
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
 * @brief ERR_SPECIFYPARAMETER_696 함수에서 사용되는 std::string 타입에 대한 설명입니다.
 *
 * 이 함수는 ERR_SPECIFYPARAMETER_696 에러 메시지를 생성하기 위해 std::string 타입을 사용합니다.
 * ERR_SPECIFYPARAMETER_696 에러는 서버, 클라이언트, 채널, 모드를 매개변수로 받아 해당 모드에 대한 파라미터를 지정해야
 * 함을 알려주는 메시지입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param channel 채널 객체
 * @param mode 모드 문자열
 * @return ERR_SPECIFYPARAMETER_696 에러 메시지
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
 * @brief ERR_NORECIPIENT_411에 대한 설명입니다.
 *
 * ERR_NORECIPIENT_411는 IRC 프로토콜에서 사용되는 응답 코드입니다.
 * 이 응답 코드는 메시지를 보낼 대상이 지정되지 않았을 때 발생합니다.
 *
 * @param server 서버 객체입니다.
 * @param client 클라이언트 객체입니다.
 * @return std::string ERR_NORECIPIENT_411에 대한 응답 메시지를 반환합니다.
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
 * @brief Response 클래스의 ERR_CANNOTSENDTOCHAN_404 함수는 서버, 클라이언트 및 채널 객체를 인자로 받아
 *        채널에 메시지를 보낼 수 없는 상황에서 발생하는 오류를 처리합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param channel 채널 객체
 * @return std::string 오류 메시지를 반환합니다.
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
 * @brief Response::RPL_WHOREPLY_352 함수는 서버, 클라이언트 및 채널 정보를 기반으로 WHO 명령에 대한 응답을 생성합니다.
 *
 * @param server 서버 객체에 대한 참조
 * @param client 클라이언트 객체에 대한 참조
 * @param channel 채널 객체에 대한 참조
 * @return 생성된 WHO 응답 문자열
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
