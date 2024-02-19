#include "Command.hpp"

Command::Command(void)
{
}

Command::Command(const Command& src)
{
}

Command::~Command(void)
{
}

Command& Command::operator=(Command const& rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

// Getter
const std::string &Command::getPrefix() const
{
    return this->m_prefix;
}

const std::string &Command::getCommand() const
{
    return this->m_command;
}

const std::vector<std::string> &Command::getParams() const
{
    return this->m_params;
}

// Setter
void Command::setPrefix(std::string &pre)
{
    this->m_prefix = pre;
}

void Command::setCommand(std::string &cmd)
{
    this->m_command = cmd;
}

void Command::setParams(std::vector<std::string> &params)
{
    this->m_params = params;
}

void Command::addParams(std::string &param)
{
    this->m_params.push_back(param);
}