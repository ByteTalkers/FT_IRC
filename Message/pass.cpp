#include "Message.hpp"

/**
 * 유저 등록 전 PASS 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 이미 비밀번호 등록했으면 => ERR_ALREADYREGISTERED_462
 * - 클라이언트 등록 PASS 플래그 켜기
*/
void Message::registerPassExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }
    if (client.getIsRegisterFlags()[PASS_REG])
    {
        client.addSendMsg(Response::ERR_ALREADYREGISTERED_462(server, client));
        client.setWriteTypes(MYSELF);
        return;
    }
    if (server.getPassword() == cmd->getParams()[0])
    {
        client.setRegisterFlags(PASS, true);
    }
}

/**
 * 유저 등록 후 PASS 실행부
 * - 파라미터 없으면 => ERR_NEEDMOREPARAMS_461
 * - 이미 등록됐기 때문에 => ERR_ALREADYREGISTERED_462
*/
void Message::passExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        client.setWriteTypes(MYSELF);
        return;
    }
    client.addSendMsg(Response::ERR_ALREADYREGISTERED_462(server, client));
    client.setWriteTypes(MYSELF);
    return;
}
