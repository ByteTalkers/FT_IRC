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
    std::vector<std::string> m_response;

  public:
    Response();
    ~Response();
    Response(const Response &src);
    Response &operator=(Response const &rhs);

    std::string rplPrefix(Server &se);
    std::string rplCRLF();


    // 유저 등록 후 응답
    std::string rplWelcome_001(Client &cl, Server &se);
    std::string rplYourHost_002(Client &cl, Server &se);
    std::string rplCreated_003(Client &cl, Server &se);
    std::string rplMyInfo_004(Client &cl, Server &se);
    std::string rplISupport_005(Client &cl, Server &se);

    // LUSER 응답
    std::string rplLUserClient_251(Client &cl, Server &se);
    // void rplLUserOp_252();
    // void rplLUserUnknown_253();
    // void rplLUserChannels_254();
    std::string rplLUserMe_255(Client &cl, Server &se);
    // void rplLocalUsers_265();
    // void rplGlobalUsers_266();

    // motd
    std::string rplMotd_372(Client &cl, Server &se);
    std::string rplMotdStart_375(Client &cl, Server &se);
    std::string rplEndOfMotd_376(Client &cl, Server &se);

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