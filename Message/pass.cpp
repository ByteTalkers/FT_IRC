#include "Message.hpp"

/**
 * @brief Message 클래스의 registerPassExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 명령어를 인자로 받아서 실행합니다.
 * 클라이언트가 전달한 명령어의 파라미터 개수가 1보다 작으면 ERR_NEEDMOREPARAMS_461 응답을 클라이언트에게 전송하고
 * 함수를 종료합니다. 서버의 비밀번호와 클라이언트가 전달한 비밀번호가 일치하지 않으면 ERR_PASSWDMISMATCH_464 응답을
 * 클라이언트에게 전송합니다. 서버의 비밀번호와 클라이언트가 전달한 비밀번호가 일치하면 클라이언트의 등록 플래그를
 * PASS_REG로 설정합니다. 서버의 비밀번호와 클라이언트가 전달한 비밀번호가 일치하지 않으면 클라이언트의 등록 플래그를
 * PASS_REG로 설정하지 않습니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd 명령어 객체
 */
void Message::registerPassExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    if (server.getPassword() != cmd->getParams()[0])
    {
        client.addSendMsg(Response::ERR_PASSWDMISMATCH_464(server, client));
    }
    if (server.getPassword() == cmd->getParams()[0])
    {
        client.setRegisterFlags(PASS_REG, true);
    }
    else
    {
        client.setRegisterFlags(PASS_REG, false);
    }
}

/**
 * @brief Message 클래스의 passExecute 함수입니다.
 *
 * 이 함수는 서버와 클라이언트, 그리고 Command 객체를 인자로 받아서 실행합니다.
 * cmd 객체의 파라미터 개수가 1보다 작으면 ERR_NEEDMOREPARAMS_461 응답을 클라이언트에게 전송하고 함수를 종료합니다.
 * 그렇지 않으면 ERR_ALREADYREGISTERED_462 응답을 클라이언트에게 전송하고 함수를 종료합니다.
 *
 * @param server 서버 객체
 * @param client 클라이언트 객체
 * @param cmd Command 객체
 */
void Message::passExecute(Server &server, Client &client, Command *cmd)
{
    if (cmd->getParamsCount() < 1)
    {
        client.addSendMsg(Response::ERR_NEEDMOREPARAMS_461(server, client, cmd->getCommand()));
        return;
    }
    client.addSendMsg(Response::ERR_ALREADYREGISTERED_462(server, client));
    return;
}
