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
  public:
    // 응답 기본 틀
    static std::string generateRpl(const std::string &command, const std::string &to, const std::string &args);
    static std::string generateMessage(const std::string &command, const std::string &args);

    // pong 응답
    static std::string pongResponse(const std::string &se_name, const std::string &token);


    // 유저 등록 후 응답
    static std::string rplWelcome_001(const std::string &nick, const std::string &user);
    static std::string rplYourHost_002(const std::string &nick, const std::string &se_name);
    static std::string rplCreated_003(const std::string &nick, time_t se_created);
    static std::string rplMyInfo_004(const std::string &nick, const std::string &se_name);
    static std::string rplISupport_005(const std::string &nick);

    // LUSER 응답
    static std::string rplLUserClient_251(const std::string &nick, const std::string &cl_count);
    static std::string rplLUserMe_255(const std::string &nick, const std::string &cl_count);

    // 아래는 should
    // void rplLUserOp_252();
    // void rplLUserUnknown_253();
    // void rplLUserChannels_254();
    // void rplLocalUsers_265();
    // void rplGlobalUsers_266();

    // motd
    static std::string rplMotd_372(const std::string &nick);
    static std::string rplMotdStart_375(const std::string &nick, const std::string &se_name);
    static std::string rplEndOfMotd_376(const std::string &nick);

    // join -> mode -> who -> mode 밴리스트 (클라이언트가 join 이후 보내는 순서)
    void rplNamReply_353();
    void rplEndofNames_366();

    // mode
    void rplChannelModeIs_324();
    void rplCreationTime_329();

    // mode banlist
    void rplBanList_367();
    void rplEndofBanList_368();

    // who
    void rplWhoReply_352();
    void rplEndOfWho_315();
};

#endif