#ifndef Command_HPP
# define Command_HPP

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Command
{
    private:
        std::string m_prefix;
        std::string m_command;
        std::vector<std::string> m_params;
    public:
        Command(void);
        Command(const Command& src);
        ~Command(void);
        Command& operator=(Command const& rhs);

        const std::string &getPrefix() const;
        const std::string &getCommand() const;
        const std::vector<std::string> &getParams() const;

        void setPrefix(std::string &pre);
        void setCommand(std::string &cmd);
        void setParams(std::vector<std::string> &params);

        void addParams(std::string &param);
};

#endif