#ifndef Command_HPP
#define Command_HPP

#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * \r\n 으로 잘려진 정보를 가지고 있음
 * - prefix
 * - command
 * - params
 */
class Command
{
  private:
    std::string m_prefix;
    std::string m_command;
    std::vector<std::string> m_params;

  public:
    Command(void);
    Command(const Command &src);
    ~Command(void);
    Command &operator=(Command const &rhs);

    /* getter */
    const std::string &getPrefix() const;
    const std::string &getCommand() const;
    const std::vector<std::string> &getParams() const;
    size_t getParamsCount() const;

    /* setter */
    void setPrefix(std::string &pre);
    void setCommand(std::string &cmd);
    void setParams(std::vector<std::string> &params);

    /* functions */
    void addParams(std::string &param);
    // void display();
};

#endif