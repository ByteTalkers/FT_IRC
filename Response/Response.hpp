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

    static std::string generateResponse(const std::string &prefix, const std::string &command, const std::string &args);

  public:
    // 응답 기본 틀

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

    // USER

    // QUIT

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