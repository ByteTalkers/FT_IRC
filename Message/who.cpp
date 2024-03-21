#include "Message.hpp"

/**
 * @brief Message 클래스의 whoExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어를 매개변수로 받아서 실행합니다.
 * 매개변수로 받은 명령어의 파라미터 개수를 확인하고, 파라미터 개수가 1보다 작으면 ERR_NEEDMOREPARAMS_461 응답을
 * 클라이언트에게 전송합니다. 파라미터로 받은 채널을 서버에서 찾고, 채널이 존재하지 않으면 ERR_NOSUCHCHANNEL_403 응답을
 * 클라이언트에게 전송합니다. 채널이 존재하면 RPL_WHOREPLY_352 응답과 RPL_ENDOFWHO_315 응답을 클라이언트에게 전송합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
 */
void Message::whoExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    Channel *channel = server.findChannel(cmd->getParams()[0]);
    if (channel == NULL)
    {
        client.addSendMsg(Response::ERR_NOSUCHCHANNEL_403(server, client, cmd->getCommand()));
        return;
    }

    client.addSendMsg(Response::RPL_WHOREPLY_352(server, client, *channel));
    client.addSendMsg(Response::RPL_ENDOFWHO_315(server, client, *channel));
}