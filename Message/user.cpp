#include "Message.hpp"

/**
 * 유저 등록 전 USER 실행부
 * - 파라미터 4개 미만 => ERR_NEEDMOREPARAMS_461
 * - 이미 유저 등록했으면 => ERR_ALREADYREGISTERED_462
 * - 클라이언트 유저 이름 세팅
 * - 클라이언트 등록 USER 플래그 켜기
 */
/**
 * @brief 사용자 등록을 처리하는 함수입니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령 객체
 */
void Message::registerUserExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 4)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    if (client.getIsRegisterFlags()[USER_REG])
    {
        client.addSendMsg(Response::ERR_ALREADYREGISTERED_462(server, client));
        return;
    }
    client.setUsername(cmd->getParams()[0]);
    client.setRealname(cmd->getParams()[3]);
    client.setRegisterFlags(USER_REG, true);
}

/**
 * @brief 사용자 명령을 처리하는 함수입니다.
 *
 * 이 함수는 클라이언트의 사용자 명령을 처리합니다.
 * 만약 파라미터 개수가 4개 미만인 경우 ERR_NEEDMOREPARAMS_461 응답을 클라이언트에게 전송하고 함수를 종료합니다.
 * 이미 유저가 등록되어 있는 경우 ERR_ALREADYREGISTERED_462 응답을 클라이언트에게 전송하고 함수를 종료합니다.
 * 클라이언트의 유저 이름을 설정하고 등록 USER 플래그를 켭니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령 객체
 */
void Message::userExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 4)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    client.addSendMsg(Response::RPL_USERHOST_302(server, client));
}