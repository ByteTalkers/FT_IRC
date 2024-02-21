#ifndef Response_HPP
#define Response_HPP

#include <string>
#include <vector>

#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../utils/utils.hpp"

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
    static std::string PONG(const std::string &se_name, const std::string &token);

    // 유저 등록 후 응답
    static std::string RPL_WELCOME_001(const std::string &se_name, const std::string &nick, const std::string &user);

    // ERROR
    static std::string ERR_NEEDMOREPARAMS_461(const std::string &se_name, const std::string &nick,
                                              std::string &command);

    // PASS, USER
    static std::string ERR_ALREADYREGISTERED_462(const std::string &se_name, const std::string &nick);

    // NICK
    static std::string ERR_NONICKNAMEGIVEN_431(const std::string &se_name, const std::string &nick);
    static std::string ERR_ERRONEUSNICKNAME_432(const std::string &se_name, const std::string &nick,
                                                const std::string &fail_nick);
    static std::string ERR_NICKNAMEINUSE_433(const std::string &se_name, const std::string &nick,
                                             const std::string &fail_nick);
    static std::string ERR_NICKCOLLISION_436(const std::string &se_name, const std::string &nick,
                                             const std::string &fail_nick);

    // QUIT

    // JOIN
    static std::string RPL_TOPIC_332(const std::string &se_name, const std::string &nick, const std::string &ch_name,
                                     const std::string &ch_topic);

    static std::string ERR_INVITEONLYCHAN_473(const std::string &se_name, const std::string &nick,
                                              const std::string &ch_name);
    static std::string ERR_BADCHANNELKEY_475(const std::string &se_name, const std::string &nick,
                                             const std::string &ch_name);
    static std::string ERR_NOSUCHCHANNEL_403(const std::string &se_name, const std::string &nick,
                                             const std::string &ch_name);
    static std::string ERR_TOOMANYCHANNELS_405(const std::string &se_name, const std::string &nick,
                                               const std::string &ch_name);

    // PART
    static std::string ERR_NOTONCHANNEL_442(const std::string &se_name, const std::string &nick,
                                            const std::string &ch_name);

    // MODE
    static std::string RPL_CHANNELMODEIS_324(const std::string &se_name, const std::string &nick,
                                             const std::string &ch_name, const std::string &ch_modes,
                                             const std::string &ch_pass);
    static std::string RPL_BANLIST_367(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string RPL_ENDOFBANLIST_368(const std::string &se_name, const std::string &nick,
                                            const std::string &ch_name);

    static std::string ERR_NOSUCHNICK_401(const std::string &se_name, const std::string &nick,
                                          const std::string &no_nick);
    static std::string ERR_KEYSET_467(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string ERR_UNKNOWNMODE_472(const std::string &se_name, const std::string &nick,
                                           const std::string &mode);
    static std::string ERR_CHANOPRIVSNEEDED_482(const std::string &se_name, const std::string &nick,
                                                const std::string &ch_name);

    // TOPIC
    static std::string RPL_NOTOPIC_331(const std::string &se_name, const std::string &nick, const std::string &ch_name);

    // LIST
    static std::string RPL_LISTSTART_321(const std::string &se_name, const std::string &nick);
    static std::string RPL_LIST_322(const std::string &se_name, const std::string &nick, const std::string &ch_name,
                                    const std::string &ch_cl_count, const std::string &ch_topic);
    static std::string RPL_LISTEND_323(const std::string &se_name, const std::string &nick);

    // INVITE
    static std::string RPL_INVITING_341(const std::string &se_name, const std::string &nick, const std::string &invited,
                                        const std::string &ch_name);

    static std::string ERR_USERONCHANNEL_443(const std::string &se_name, const std::string &nick,
                                             const std::string &invited, const std::string &ch_name);

    // KICK

    // PRIVMSG
    static std::string ERR_NORECIPIENT_411(const std::string &se_name, const std::string &nick);
    static std::string ERR_NOTEXTTOSEND_412(const std::string &se_name, const std::string &nick);
    static std::string ERR_CANNOTSENDTOCHAN_404(const std::string &se_name, const std::string &nick,
                                                const std::string &ch_name);
    static std::string ERR_TOOMANYTARGETS_407(const std::string &se_name, const std::string &nick);

    // WHO
    static std::string RPL_WHOREPLY_352(const std::string &se_name, const std::string &nick, const std::string &ch_name,
                                        const std::string &user);
    static std::string RPL_ENDOFWHO_315(const std::string &se_name, const std::string &nick,
                                        const std::string &ch_name);

    // PING, PONG
    static std::string ERR_NOORIGIN_409(const std::string &se_name, const std::string &nick);

    // MODE user +i
    static std::string RPL_UMODEIS_221(const std::string &user, const std::string &nick);

    // WHOIS
    static std::string RPL_WHOISUSER_311(const std::string &se_name, const std::string &nick, const std::string &user);
    static std::string RPL_WHOISSERVER_312(const std::string &se_name, const std::string &nick);
    static std::string RPL_ENDOFWHOIS_318(const std::string &se_name, const std::string &nick);
};

#endif