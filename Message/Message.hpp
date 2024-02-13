#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <string>
#include <vector>

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

    bool crlfCheck();
    void seperateOrigin();

    /** Getter */
    const std::string getOrigin() const;
    const std::string getPrefix() const;
    const std::string getCommand() const;
    const std::vector<std::string> getParams() const;

    /** Setter */
    void setOrigin(std::string &origin);
    void setPrefix(std::string &prefix);
    void setCommand(std::string &command);
    void setParams(std::vector<std::string> &params);

    /** test */
    void display() const;
};

#endif