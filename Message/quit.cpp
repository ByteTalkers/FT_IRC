#include "Message.hpp"

void Message::quitExecute(Server &server, Client &client, Command *cmd)
{
    std::string reason = " :leaving"; // "떠나는 이유" 초기화
    if (!cmd->getParams().empty())    // "떠나는 이유" 필요 시 변경
        reason = cmd->getParams()[0];
    client.setLeaveMsg(reason);
    (void)server;
}