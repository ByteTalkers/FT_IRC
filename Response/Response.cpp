#include "Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

std::string Response::rplPrefix(Server &se)
{
    // 서버이름
    std::string pre = ":";
    pre += se.getName();
    return pre;
}

std::string Response::rplCRLF()
{
    return "\r\n";
}

std::string Response::rplWelcome_001(Client &cl, Server &se)
{
    // :irc.local 001 hj :Welcome to the Localnet IRC Network hj!root@127.0.0.1
    // " 001 " + nickname + ":welcome message " + nickname + "!" + username + "@" + ip
    std::string rpl = rplPrefix(se);
    rpl += " 001 ";
    rpl += cl.getNick();
    rpl += " :Welcom to the FT_IRC Network ";
    rpl += cl.getNick();
    rpl += "!";
    rpl += cl.getUser();
    rpl += "@";
    // todo : getIp();
    // rpl += cl.getIp();
    return rpl;
}

std::string Response::rplYourHost_002(Client &cl, Server &se)
{
    // :irc.local 002 hj :Your host is irc.local, running version InspIRCd-3
    std::string rpl = rplPrefix(se);
    rpl += " 002 ";
    rpl += cl.getNick();
    rpl += " :Your host is ";
    rpl += se.getName();
    rpl += ", running version ";
    rpl += se.getVersion();

    return rpl;
}

std::string Response::rplCreated_003(Client &cl, Server &se)
{
    //: irc.local 003 hj :This server was created 00:34:38 Feb 13 2024
    std::string rpl = rplPrefix(se);
    rpl += " 003 ";
    rpl += cl.getNick();
    rpl += " :This server was created ";
    rpl += timeToString(se.getCreated());

    return rpl;
}

std::string Response::rplMyInfo_004(Client &cl, Server &se)
{
    //: irc.local 004 hj irc.local InspIRCd-3 iosw biklmnopstv :bklov
    std::string rpl = rplPrefix(se);
    rpl += " 004 ";
    rpl += cl.getNick();
    rpl += " ";
    rpl += se.getName();
    rpl += " ";
    rpl += se.getVersion();
    // available usermode
    // available channelmode
    rpl += " iklt ";
    // channel mode with parameters
    rpl += ":kl";

    return rpl;
}

std::string Response::rplISupport_005(Client &cl, Server &se)
{
    //: irc.local 005 hj AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=#
    //: ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server irc.local
    //: 005 hj MAXTARGETS=20 MODES=20 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307
    //: USERLEN=10 WHOX :are supported by this server

    std::string rpl = rplPrefix(se);

    rpl += " 005 ";
    rpl += cl.getNick();

    // todo: 서버 설정 내용 / parameter=value 형식으로
    // 서버 객체에서 가져올지?
    // 서버 객체는 이런 설정들을 config 파일에서 따로 관리할지?
    rpl += " CHANLIMIT=#:20 CHANNELLEN=64 NICKLEN=30";
    rpl += " :are supported by this server";

    return rpl;
}

std::string Response::rplLUserClient_251(Client &cl, Server &se)
{
    // "<client> :There are <u> users and <i> invisible on <s> servers"
    std::string rpl = rplPrefix(se);

    rpl += " 251 ";
    rpl += cl.getNick();
    // server cl count?
    rpl += " There are 0 users and ";
    // server cl count?
    rpl += "1";
    rpl += " invisible on 1 servers";

    return rpl;
}

std::string Response::rplLUserMe_255(Client &cl, Server &se)
{
    // "<client> :I have <c> clients and <s> servers"
    std::string rpl = rplPrefix(se);

    rpl += " 255 ";
    rpl += cl.getNick();
    rpl += " :I have ";
    // server cl count
    rpl += "2";
    rpl += " clients and 0 servers";
    return rpl;
}

std::string Response::rplMotd_372(Client &cl, Server &se)
{
    // "<client> :<line of the motd>"
    // :irc.local 372 nickname :- **************************************************
    // :irc.local 372 nickname :- *             H    E    L    L    O              *
    // :irc.local 372 nickname :- *  This is a private irc server. Please contact  *
    // :irc.local 372 nickname :- *  the admin of the server for any questions or  *
    // :irc.local 372 nickname :- *  issues.                                       *
    // :irc.local 372 nickname :- **************************************************
    // :irc.local 372 nickname :- *  The software was provided as a package of     *
    // :irc.local 372 nickname :- *  Debian GNU/Linux <https://www.debian.org/>.   *
    // :irc.local 372 nickname :- *  However, Debian has no control over this      *
    // :irc.local 372 nickname :- *  server.                                       *
    // :irc.local 372 nickname :- **************************************************
    // :irc.local 372 nickname :- (The sysadmin possibly wants to edit </etc/inspircd/inspircd.motd>)

    std::string rpl = rplPrefix(se);

    // todo: 여러번 나눠서 보내야함
    rpl += " 372 ";
    rpl += "**************************************************\n";
    rpl += "*             H    E    L    L    O              *\n";
    rpl += "**************************************************\n";

    return rpl;
}

std::string Response::rplMotdStart_375(Client &cl, Server &se)
{
    // "<client> :- <server> Message of the day - "
    // :irc.local 375 nickname :irc.local message of the day
    std::string rpl = rplPrefix(se);
    rpl += " 372 ";
    rpl += cl.getNick();
    rpl += " ";
    rpl += rplPrefix(se);
    rpl += " message of the day";

    return rpl;
}

std::string Response::rplEndOfMotd_376(Client &cl, Server &se)
{
    // "<client> :End of /MOTD command."
    // :irc.local 376 nickname :End of message of the day.
    std::string rpl = rplPrefix(se);
    rpl += " 376 ";
    rpl += cl.getNick();
    rpl += " :End of message of the day";

    return rpl;
}
