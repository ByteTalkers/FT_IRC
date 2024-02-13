#include "Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

std::string &Response::rplPrefix()
{
    // ":" + source
}

void Response::rplWelcome_001()
{
    // :irc.local 001 hj :Welcome to the Localnet IRC Network hj!root@127.0.0.1
    // " 001 " + nickname + ":welcome message " + nickname + "!" + username + "@" + ip
}

void Response::rplYourHost_002()
{
    // :irc.local 002 hj :Your host is irc.local, running version InspIRCd-3
}

void Response::rplCreated_003()
{
    //:irc.local 003 hj :This server was created 00:34:38 Feb 13 2024
}

void Response::rplMyInfo_004()
{
    //:irc.local 004 hj irc.local InspIRCd-3 iosw biklmnopstv :bklov

}

void Response::rplISupport_005()
{
    //:irc.local 005 hj AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server
    //:irc.local 005 hj MAXTARGETS=20 MODES=20 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307 USERLEN=10 WHOX :are supported by this server

}