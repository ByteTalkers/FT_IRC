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


    // PASS
    // err
    static std::string errNeedMoreParams_461();


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