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
#include "Command.hpp"

class Client;
class Server;
class Channel;

enum eCmds
{
    CAP,
    PASS,
    NICK,
    USER,
    PING,
    QUIT,
    JOIN,
    PART,
    MODE,
    TOPIC,
    INVITE,
    PRIVMSG,
    WHOIS,
    KICK
};

class Message
{
  private:
    std::string m_origin;

    std::vector<Command *> m_cmds;

  public:
    Message();
    Message(const std::string &origin);
    ~Message();
    Message(const Message &src);
    Message &operator=(Message const &rhs);

    bool crlfCheck();
    void parsingOrigin();

    void execute(Server &server, Client &client);

    // register
    void registerPassExecute(Server &server, Client &client, Command *cmd);
    void registerNickExecute(Server &server, Client &client, Command *cmd);
    void registerUserExecute(Server &server, Client &client, Command *cmd);

    // Command
    void registerExecute(Server &server, Client &client, Command *cmd);
    void commandExecute(Server &server, Client &client, Command *cmd);

    void joinExecute(Server &server, Client &client, Command *cmd);
    void passExecute(Server &server, Client &client, Command *cmd);
    void nickExecute(Server &server, Client &client, Command *cmd);
    void userExecute(Server &server, Client &client, Command *cmd);
    void modeExecute(Server &server, Client &client, Command *cmd);
    void whoisExecute(Server &server, Client &client, Command *cmd);
    void pingExecute(Server &server, Client &client, Command *cmd);
    void partExecute(Server &server, Client &client, Command *cmd);
    void quitExecute(Server &server, Client &client, Command *cmd);
    void privmsgExecute(Server &server, Client &client, Command *cmd);
    void kickExecute(Server &server, Client &client, Command *cmd);
    void inviteExecute(Server &server, Client &client, Command *cmd);
    void topicExecute(Server &server, Client &client, Command *cmd);

    // Getter
    const std::string &getOrigin() const;
    const std::vector<Command *> &getCmds() const;

    // Setter
    void setOrigin(std::string &origin);
    void setCmds(std::vector<Command *> &cmds);

    // test
    void display();
};

#endif