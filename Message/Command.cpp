#include "Command.hpp"

Command::Command(void) : m_prefix(""), m_command(""), m_params_count(0)
{
}

Command::~Command(void)
{
}

Command::Command(const Command &src)
{
    *this = src;
}

Command &Command::operator=(Command const &rhs)
{
    if (this != &rhs)
    {
        m_prefix = rhs.m_prefix;
        m_command = rhs.m_command;
        m_params = rhs.m_params;
        m_params_count = rhs.m_params_count;
    }
    return *this;
}

/* getter */
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

const std::size_t &Command::getParamsCount() const
{
    return this->m_params_count;
}

/* setter */
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

void Command::setParamsCount(std::size_t &count)
{
    this->m_params_count = count;
}

/**
 * m_params에 param을 하나씩 추가하는 함수
 */
void Command::addParams(std::string &param)
{
    this->m_params.push_back(param);
    this->m_params_count++;
}

/**
 * Command 멤버변수 보여주는 함수
 */
void Command::display()
{
    std::cout << "prefix: " << m_prefix << std::endl;
    std::cout << "command: " << m_command << std::endl;
    for (std::size_t i = 0; i < m_params.size(); i++)
    {
        std::cout << "param " << i << ": " << m_params[i] << std::endl;
    }
}