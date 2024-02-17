#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../Response/Response.hpp"
#include "../Server/Server.hpp"

class Client;
class Server;
class Channel;

class Message
{
  private:
    std::string m_origin;
    std::string m_prefix;
    std::string m_command;
    std::vector<std::string> m_params;

  public:
    Message(std::string &origin);
    ~Message();
    Message(const Message &src);
    Message &operator=(Message const &rhs);

    bool crlfCheck();
    void seperateOrigin();

    // Command
    void commandExecute(Server &server, Client &client);

    void topicExecute(Server &server, Client &client);
    void modeExecute(Server &server, Client &client);

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