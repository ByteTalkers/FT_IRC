#include "Message.hpp"

/**
 * ping 실행부
 * 파라미터 있으면 => 해당 파라미터를 담아서 pong 응답
 * 파라미터 없으면 => ""를 담아서 응답
*/
void Message::pingExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParams().empty())
    {
        client.addSendMsg(Response::PONG(server, ""));
    }
    else
    {
        client.addSendMsg(Response::PONG(server, cmd->getParams()[0]));
    }
}