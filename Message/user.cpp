#include "Message.hpp"

/**
 * 유저 등록 전 USER 실행부
 * - 파라미터 4개 미만 => ERR_NEEDMOREPARAMS_461
 * - 이미 유저 등록했으면 => ERR_ALREADYREGISTERED_462
 * - 클라이언트 유저 이름 세팅
 * - 클라이언트 등록 USER 플래그 켜기
 */
/**
 * Registers a user on the server.
 *
 * This function is responsible for registering a user on the server. It checks if the command has enough parameters,
 * and if the client is already registered. If the conditions are met, it sets the username, realname, and register
 * flags for the client.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object.
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
 * Executes the USER command.
 * This function is responsible for handling the USER command received from a client.
 * It checks if the command has enough parameters, and if not, sends an error response to the client.
 * If the command has enough parameters, it adds a response message to the client's send queue.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object representing the USER command.
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