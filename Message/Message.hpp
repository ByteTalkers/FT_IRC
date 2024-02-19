#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../Client/Client.hpp"
#include "../Response/Response.hpp"
#include "../Server/Server.hpp"
#include "Command.hpp"

class Client;
class Server;

class Message
{
  private:
    std::string m_origin;
    std::string m_prefix;
    std::string m_command;
    std::vector<std::string> m_params;

    std::vector<Command*> m_inputs;

  public:
    Message(std::string &origin);
    ~Message();
    Message(const Message &src);
    Message &operator=(Message const &rhs);

    bool crlfCheck();
    void parsingOrigin();

    // Command
    void registerExecute(Server &server, Client &client);

    void commandExecute(Server &server, Client &client);

    void passExecute(Server &server, Client &client);
    void nickExecute(Server &server, Client &client);
    void userExecute(Server &server, Client &client);

    // Getter
    const std::string &getOrigin() const;
    const std::string &getPrefix() const;
    const std::string &getCommand() const;
    const std::vector<std::string> &getParams() const;

    // Setter
    void setOrigin(std::string &origin);
    void setPrefix(std::string &prefix);
    void setCommand(std::string &command);
    void setParams(std::vector<std::string> &params);

    // test
    void display();
};

#endif