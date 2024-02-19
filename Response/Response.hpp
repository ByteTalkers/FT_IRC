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
    static std::string generateResponse(const std::string &prefix, const std::string &command, const std::string &args);

  public:
    // pong 응답
    static std::string pongResponse(const std::string &se_name, const std::string &token);

    

    // 유저 등록 후 응답
    static std::string rplWelcome_001(const std::string &se_name, const std::string &nick, const std::string &user);

    // ERROR
    static std::string errNeedMoreParams_461(const std::string &se_name, const std::string &nick, std::string &command);

    // PASS, USER
    static std::string errAlreadyRegistered_462(const std::string &se_name, const std::string &nick);
    
    // NICK
    static std::string errNoNicknameGiven_431(const std::string &se_name, const std::string &nick);
    static std::string errErroneusNickname_432(const std::string &se_name, const std::string &nick, const std::string &fail_nick);
    static std::string errNickNameInUse_433(const std::string &se_name, const std::string &nick, const std::string &fail_nick);
    static std::string errNickCollision_436(const std::string &se_name, const std::string &nick, const std::string &fail_nick);

    // QUIT

    // JOIN
    static std::string rplTopic_332(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_topic);

    static std::string errInviteOnlyChan_473(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string errBadChannelKey_475(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string errNoSuchChannel_403(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string errTooManyChannels_405(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    
    // PART
    static std::string errNoToNChannel_442(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    
    // MODE
    static std::string rplChannelModeIs_324(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_modes, const std::string &ch_pass);
    static std::string rplBanList_367(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string rplEndofBanList_368(const std::string &se_name, const std::string &nick, const std::string &ch_name);

    static std::string errNoSuchNick_401(const std::string &se_name, const std::string &nick, const std::string &no_nick);
    static std::string errKeySet_467(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string errUnknownMode_472(const std::string &se_name, const std::string &nick, const std::string &mode);
    static std::string errChanOPrivsNeeded_482(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    
    // TOPIC
    static std::string rplNoTopic_331(const std::string &se_name, const std::string &nick, const std::string &ch_name);

    // LIST
    static std::string rplListStart_321(const std::string &se_name, const std::string &nick);
    static std::string rplList_322(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &ch_cl_count, const std::string &ch_topic);
    static std::string rplListEnd_323(const std::string &se_name, const std::string &nick);

    // INVITE
    static std::string rplInviting_341(const std::string &se_name, const std::string &nick, const std::string &invited, const std::string &ch_name);

    static std::string errUserOnChannel_443(const std::string &se_name, const std::string &nick, const std::string &invited, const std::string &ch_name);
    
    // KICK

    // PRIVMSG
    static std::string errNoRecipient_411(const std::string &se_name, const std::string &nick);
    static std::string errNoTextToSend_412(const std::string &se_name, const std::string &nick);
    static std::string errCanNotSendToChan_404(const std::string &se_name, const std::string &nick, const std::string &ch_name);
    static std::string errTooManyTargets_407(const std::string &se_name, const std::string &nick);

    // WHO
    static std::string rplWhoReply_352(const std::string &se_name, const std::string &nick, const std::string &ch_name, const std::string &user);
    static std::string rplEndOfWho_315(const std::string &se_name, const std::string &nick, const std::string &ch_name);

    // PING, PONG
    static std::string errNoOrigin_409(const std::string &se_name, const std::string &nick);


    // MODE user +i
    static std::string rplUmodeIs_221(const std::string &user, const std::string &nick);

    // WHOIS
    static std::string rplWhoIsUser_311(const std::string &se_name, const std::string &nick, const std::string &user);
    static std::string rplWhoIsServer_312(const std::string &se_name, const std::string &nick);
    static std::string rplEndofWhoIs_318(const std::string &se_name, const std::string &nick);
};

#endif