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

    std::vector<Command*> m_cmds;

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
    const std::vector<Command*> &getCmds() const;

    // Setter
    void setOrigin(std::string &origin);
    void setCmds(std::vector<Command*> &cmds);


    // test
    void display();
};

#endif