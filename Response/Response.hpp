#ifndef Response_HPP
#define Response_HPP

#include <string>
#include <vector>

#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../utils/utils.hpp"

class Server;
class Client;
class Channel;

class Response
{
  private:
    Response();
    ~Response();
    Response(const Response &src);
    Response &operator=(Response const &rhs);

    // 응답 기본 틀

  public:
    static std::string GENERATE(const std::string &prefix, const std::string &cmd, const std::string &args);
    // pong 응답
    static std::string PONG(Server &server, const std::string &token);

    // 유저 등록 후 응답
    static std::string RPL_WELCOME_001(Server &server, Client &client);

    // ERROR
    static std::string ERR_NEEDMOREPARAMS_461(Server &server, Client &client, const std::string &command);

    // PASS, USER
    static std::string ERR_ALREADYREGISTERED_462(Server &server, Client &client);
    static std::string RPL_USERHOST_302(Server &server, Client &client);

    // NICK
    static std::string ERR_NONICKNAMEGIVEN_431(Server &server, Client &client);
    static std::string ERR_ERRONEUSNICKNAME_432(Server &server, Client &client, const std::string &fail_nick);
    static std::string ERR_NICKNAMEINUSE_433(Server &server, Client &client, const std::string &fail_nick);
    static std::string ERR_NICKCOLLISION_436(Server &server, Client &client, const std::string &fail_nick);

    // QUIT

    // JOIN

    // :a!root@127.0.0.1 JOIN :#a
    // :irc.local 353 a = #a :@a
    // :irc.local 366 a #a :End of /NAMES list.
    static std::string RPL_TOPIC_332(Server &server, Client &client, Channel &channel);

    static std::string ERR_INVITEONLYCHAN_473(Server &server, Client &client, Channel &channel);
    static std::string ERR_BADCHANNELKEY_475(Server &server, Client &client, Channel &channel);
    static std::string ERR_NOSUCHCHANNEL_403(Server &server, Client &client, const std::string &ch_name);
    static std::string ERR_TOOMANYCHANNELS_405(Server &server, Client &client, Channel &channel);
    static std::string ERR_CHANNEL_IS_FULL_471(Server &server, Client &client, Channel &channel);

    static std::string RPL_NAMREPLY_353(Server &server, Client &client, Channel &channel);
    static std::string RPL_ENDOFNAMES_366(Server &server, Client &client, Channel &channel);
    // PART
    static std::string ERR_NOTONCHANNEL_442(Server &server, Client &client, Channel &channel);

    // MODE
    static std::string RPL_CHANNELMODEIS_324(Server &server, Client &client, Channel &channel);
    static std::string RPL_CREATIONTIME_329(Server &server, Client &client, Channel &channel);
    static std::string RPL_BANLIST_367(Server &server, Client &client, Channel &channel);
    static std::string RPL_ENDOFBANLIST_368(Server &server, Client &client, Channel &channel);

    static std::string ERR_NOSUCHNICK_401(Server &server, Client &client, const std::string &no_nick);
    static std::string ERR_KEYSET_467(Server &server, Client &client, Channel &channel);
    static std::string ERR_UNKNOWNMODE_472(Server &server, Client &client, const std::string &mode);
    static std::string ERR_CHANOPRIVSNEEDED_482(Server &server, Client &client, Channel &channel);

    static std::string ERR_SPECIFYPARAMETER_696(Server &server, Client &client, Channel &channel,
                                                const std::string &mode);

    // TOPIC
    static std::string RPL_NOTOPIC_331(Server &server, Client &client, Channel &channel);

    // LIST
    static std::string RPL_LISTSTART_321(Server &server, Client &client);
    static std::string RPL_LIST_322(Server &server, Client &client, Channel &channel);
    static std::string RPL_LISTEND_323(Server &server, Client &client);

    // INVITE
    static std::string RPL_INVITING_341(Server &server, Client &client, Client &invite, Channel &channel);

    static std::string ERR_USERONCHANNEL_443(Server &server, Client &client, Client &invite, Channel &channel);

    // KICK

    // PRIVMSG
    static std::string ERR_NORECIPIENT_411(Server &server, Client &client);
    static std::string ERR_NOTEXTTOSEND_412(Server &server, Client &client);
    static std::string ERR_CANNOTSENDTOCHAN_404(Server &server, Client &client, Channel &channel);
    static std::string ERR_TOOMANYTARGETS_407(Server &server, Client &client);

    // WHO
    static std::string RPL_WHOREPLY_352(Server &server, Client &client, Channel &channel);
    static std::string RPL_ENDOFWHO_315(Server &server, Client &client, Channel &channel);

    // MODE user +i
    static std::string RPL_UMODEIS_221(Client &client);

};

#endif