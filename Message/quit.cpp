#include "Message.hpp"

/**
 * Executes the quit command for a client.
 *
 * This function sets the leave message for the client, indicating the reason for leaving.
 * If a reason is provided as a parameter in the command, it will be used as the leave message.
 * Otherwise, a default leave message ":leaving" will be used.
 *
 * @param client The client object for which the quit command is executed.
 * @param cmd The quit command object containing the parameters.
 */
void Message::quitExecute(Client &client, Command *cmd)
{
    std::string reason = " :leaving"; // "떠나는 이유" 초기화
    if (!cmd->getParams().empty())    // "떠나는 이유" 필요 시 변경
        reason = cmd->getParams()[0];
    client.setLeaveMsg(reason);
}