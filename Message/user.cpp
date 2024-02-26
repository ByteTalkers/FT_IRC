#include "Message.hpp"

/**
 * 유저 등록 전 USER 실행부
 * - 파라미터 4개 미만 => ERR_NEEDMOREPARAMS_461
 * - 이미 유저 등록했으면 => ERR_ALREADYREGISTERED_462
 * - 클라이언트 유저 이름 세팅
 * - 클라이언트 등록 USER 플래그 켜기
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
 * 유저 등록 후 USER 실행부
 * - 파라미터 1개 미만 => ERR_NEEDMOREPARAMS_461
 * - 이미 유저 등록했으면 => ERR_ALREADYREGISTERED_462
 * - 클라이언트 유저 이름 세팅
 * - 클라이언트 등록 USER 플래그 켜기
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