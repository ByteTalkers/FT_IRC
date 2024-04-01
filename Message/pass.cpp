#include "Message.hpp"

/**
 * Registers the execution of the PASS command.
 * This function checks if the command has the required parameters and if the password matches the server's password.
 * If the password matches, it sets the PASS_REG flag for the client.
 * If the password does not match, it sends an error message to the client.
 *
 * @param server The server object.
 * @param client The client object.
 * @param cmd The command object.
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
 * Executes the "pass" command for a specific client.
 *
 * This function is responsible for executing the "pass" command for a specific client.
 * It takes a reference to a Server object, a reference to a Client object, and a pointer to a Command object as
 * parameters.
 *
 * @param server The Server object representing the server.
 * @param client The Client object representing the client.
 * @param cmd    A pointer to the Command object representing the command.
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
