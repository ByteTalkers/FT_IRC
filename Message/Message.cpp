#include "Message.hpp"

Message::Message(std::string &origin) : m_origin(origin)
{
}

Message::~Message()
{
}

bool Message::crlfCheck()
{
    std::string crlf;

    crlf = this->m_origin.substr(this->m_origin.length() - 2);
    if (crlf == "\r\n") {
        this->m_origin = this->m_origin.substr(0, this->m_origin.length() - 2);
        return true;
    }
    return false;
}

void Message::seperateOrigin()
{
    std::size_t start;
    std::size_t pos;

    bool is_prefix;
    bool is_command;
    bool is_params;

    is_prefix = true;
    is_command = false;
    is_params = false;

    pos = this->m_origin.find(' ');
    while (pos != std::string::npos)
    {
        std::string tmp;
        tmp = m_origin.substr(start, pos);

        if (is_prefix)
        {
            is_prefix = false;
            is_command = true;
            if (tmp[0] == ':')
            {
                this->m_prefix = tmp;
            }
        }
        else if (is_command)
        {
            is_command = false;
            is_params = true;
            this->m_command = tmp;
        }
        else if (is_params)
        {
            this->m_params.push_back(tmp);
        }

        pos = this->m_origin.find(' ', pos + 1);
    }
}

/** Getter */

const std::string Message::getOrigin() const
{
    return this->m_origin;
}

const std::string Message::getPrefix() const
{
    return this->m_prefix;
}

const std::string Message::getCommand() const
{
    return this->m_command;
}

const std::vector<std::string> Message::getParams() const
{
    return this->m_params;
}

/** Setter */

void Message::setOrigin(std::string &origin)
{
    this->m_origin = origin;
}
void Message::setPrefix(std::string &prefix)
{
    this->m_prefix = prefix;
}
void Message::setCommand(std::string &command)
{
    this->m_command = command;
}
void Message::setParams(std::vector<std::string> &params)
{
    this->m_params = params;
}