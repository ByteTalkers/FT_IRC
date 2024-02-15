#include "Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

std::string Response::generateRpl(const std::string &command, const std::string &to, const std::string &args)
{
    return command + " " + to + " " + args + "\r\n";
}

std::string Response::generateMessage(const std::string &command, const std::string &args)
{
    return command + " " + args + "\r\n";
}

// pong 응답
std::string Response::pongResponse(const std::string &se_name, const std::string &token)
{
    return generateMessage("PONG", se_name + " :" + token);
}


std::string Response::rplWelcome_001(const std::string &nick, const std::string &user)
{
    // :irc.local 001 hj :Welcome to the Localnet IRC Network hj!root@127.0.0.1
    // " 001 " + nickname + ":welcome message " + nickname + "!" + username + "@" + ip

    return generateRpl("001", nick, ":Welcome to the FT-IRC Network " + nick + "!" + user);
}

std::string Response::rplYourHost_002(const std::string &nick, const std::string &se_name)
{
    // :irc.local 002 hj :Your host is irc.local, running version InspIRCd-3

    return generateRpl("002", nick, ":Your host is " + se_name);
}

std::string Response::rplCreated_003(const std::string &nick, time_t se_created)
{
    //: irc.local 003 hj :This server was created 00:34:38 Feb 13 2024

    return generateRpl("003", nick, ":This server was created " + timeToString(se_created));
}

std::string Response::rplMyInfo_004(const std::string &nick, const std::string &se_name)
{
    //: irc.local 004 hj irc.local InspIRCd-3 iosw biklmnopstv :bklov

    return generateRpl("004", nick, se_name + " iklt :kl");
}

std::string Response::rplISupport_005(const std::string &nick)
{
    //: irc.local 005 hj AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=#
    //: ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server irc.local
    //: 005 hj MAXTARGETS=20 MODES=20 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307
    //: USERLEN=10 WHOX :are supported by this server

    // todo: 서버 설정 내용 / parameter=value 형식으로
    // 서버 객체에서 가져올지?
    // 서버 객체는 이런 설정들을 config 파일에서 따로 관리할지?

    return generateRpl("005", nick, " CHANLIMIT=#:20 CHANNELLEN=64 NICKLEN=30 :are supported by this server");
}

std::string Response::rplLUserClient_251(const std::string &nick, const std::string &cl_count)
{
    // "<client> :There are <u> users and <i> invisible on <s> servers"
    // todo: server clnt count

    return generateRpl("251", nick, ":There are " + cl_count + " users on 1 servers");
}

std::string Response::rplLUserMe_255(const std::string &nick, const std::string &cl_count)
{
    // "<client> :I have <c> clients and <s> servers"
    // todo: server clnt count

    return generateRpl("255", nick, ":I have " + cl_count + " clients and 0 servers");
}

std::string Response::rplMotd_372(const std::string &nick)
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

    // todo: 여러번 나눠서 보내야함 어떻게?

    return generateRpl("372", nick, ":- *             H    E    L    L    O              *");
}

std::string Response::rplMotdStart_375(const std::string &nick, const std::string &se_name)
{
    // "<client> :- <server> Message of the day - "
    // :irc.local 375 nickname :irc.local message of the day

    return generateRpl("375", nick, ":" + se_name + " Message of the day");
}

std::string Response::rplEndOfMotd_376(const std::string &nick)
{
    // "<client> :End of /MOTD command."
    // :irc.local 376 nickname :End of message of the day.

    return generateRpl("376", nick, ":End of message of the day");
}
