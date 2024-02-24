#include "Message.hpp"

static void parsingSpace(std::string &split, Command *cmd);
static void setCmd(std::string &split, Command *cmd);

// \n 기준으로 파싱
void Message::parsingOrigin()
{
    std::istringstream iss(this->m_origin);
    std::string buffer;

    while (std::getline(iss, buffer, '\n'))
    {
        std::string split = buffer;
        if (split[split.length() - 1] == '\r')
        {
            split.pop_back();
        }

        Command cmd;

        // space 기준으로 스플릿
        parsingSpace(split, &cmd);
        this->m_cmds.push_back(cmd);
    }
}

static void parsingSpace(std::string &split, Command *cmd)
{
    std::istringstream iss(split);
    std::string buffer;
    bool is_first = true;

    while (std::getline(iss, buffer, ' '))
    {
        std::string split = buffer;
        if (is_first && split[0] == ':')
        {
            split.substr(1);
            cmd->addParams(split);
            is_first = false;
        }
        else
        {
            setCmd(split, cmd);
            is_first = false;
        }
    }
}

static void setCmd(std::string &split, Command *cmd)
{
    // 커맨드가 비어있으면 커맨드 먼저 채우기
    if (cmd->getCommand().empty())
    {
        for (std::size_t i = 0; i < split.length(); i++)
        {
            split[i] = std::toupper(split[i]);
        }
        cmd->setCommand(split);
    }
    else
    {
        // 파라미터 : 붙어있는 경우 떼어주고 넣어줌
        if (split[0] == ':')
        {
            std::string subTmp = split.substr(1);
            cmd->addParams(subTmp);
        }
        else
        {
            cmd->addParams(split);
        }
    }
}