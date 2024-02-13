#ifndef Response_HPP
#define Response_HPP

#include <string>

class Response
{
  private:
    std::string m_response;

  public:
    Response(void);
    ~Response(void);

    /** 유저 등록 후 응답 */
    void rplWelcome_001();
    void rplYourHost_002();
    void rplCreated_003();
    void rplMyInfo_004();
    void rplISupport_005();

    /** LUSER 응답 */
    void rplLUserClient_251();
    void rplLUserOp_252();
    void rplLUserUnknown_253();
    void rplLUserChannels_254();
    void rplLUserMe_255();
    void rplLocalUsers_265();
    void rplGlobalUsers_266();

    /** motd */
    void rplMotd_372();
    void rplMotdStart_375();
    void rplEndOfMotd_376();

    /** join -> mode -> who -> mode #c b*/
    void rplNamReply_353();
    void rplEndofNames_366();

    /** mode */
    void rplChannelModeIs_324();
    void rplCreationTime_329();
    
    /** mode banlist */
    void rplBanList_367();
    void rplEndofBanList_368();

    /** who */
    void rplWhoReply_352();
    void rplEndOfWho_315();

};

#endif