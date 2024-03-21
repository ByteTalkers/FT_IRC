#include "Message.hpp"

/**
 * @brief 클라이언트가 종료 명령을 실행할 때 호출되는 함수입니다.
 *
 * @param client 종료 명령을 실행하는 클라이언트 객체
 * @param cmd 종료 명령 객체
 */
void Message::quitExecute(Client &client, Command *cmd)
{
    std::string reason = " :leaving"; // "떠나는 이유" 초기화
    if (!cmd->getParams().empty())    // "떠나는 이유" 필요 시 변경
        reason = cmd->getParams()[0];
    client.setLeaveMsg(reason);
}