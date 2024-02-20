#ifndef Command_HPP
#define Command_HPP

#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Command
{
  private:
    std::string m_prefix;
    std::string m_command;
    std::vector<std::string> m_params;
    std::size_t m_params_count;

  public:
    Command(void);
    Command(const Command &src);
    ~Command(void);
    Command(const std::string &command, const std::vector<std::string> &params); // test를 위해 잠시 추가
    Command &operator=(Command const &rhs);

    const std::string &getPrefix() const;
    const std::string &getCommand() const;
    const std::vector<std::string> &getParams() const;
    const std::size_t &getParamsCount() const;

    void setPrefix(std::string &pre);
    void setCommand(std::string &cmd);
    void setParams(std::vector<std::string> &params);
    void setParamsCount(std::size_t &count);

    void addParams(std::string &param);

    void display();
};

#endif