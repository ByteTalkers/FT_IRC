#include "Message.hpp"

/**
 * @brief Message 클래스의 pingExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트 간의 PING-PONG 메시지를 처리합니다.
 * 만약 cmd의 파라미터가 비어있는 경우, 서버에 빈 문자열을 포함한 PONG 메시지를 보냅니다.
 * 그렇지 않은 경우, cmd의 첫 번째 파라미터를 포함한 PONG 메시지를 보냅니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd Command 객체
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