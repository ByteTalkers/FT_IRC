#include "Command.hpp"

Command::Command(void)
{
}

Command::~Command(void)
{
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

void Command::display()
{
    std::cout << "prefix: " << m_prefix <<std::endl;
    std::cout << "command: " << m_command <<std::endl;
    for (std::size_t i = 0; i < m_params.size(); i++)
    {
        std::cout << "param " << i << ": " << m_params[i] << std::endl;
    }
}