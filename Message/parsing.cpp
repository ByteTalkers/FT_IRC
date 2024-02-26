#include "Message.hpp"

static void parsingSpace(std::string &split, Command *cmd);
static void setCmd(std::string &split, Command *cmd);

/**
 * 클라이언트가 보낸 메시지 파싱하는 부분
 * - \n 기준으로 파싱하고 \r가 붙어있다면 떼줌
 * - 스페이스 기준으로 스플릿
 */
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

        parsingSpace(split, &cmd);
        this->m_cmds.push_back(cmd);
    }
}

/**
 * 스페이스 기준으로 스플릿해줌
 * - 첫번째 인자에 ':' 가 붙어있다면 => prefix
 * - 다음 인자부터는 커맨드가 비어있다면 => command
 * - ':'가 붙은 인자가 나오면 거기부터는 스페이스가 포함될 수 있는 last 인자 => 스페이스까지 다 넣어줌
 */
static void parsingSpace(std::string &split, Command *cmd)
{
    std::istringstream iss(split);
    std::string buffer;
    std::string last;
    bool is_prefix_exist = false;
    bool is_first = true;
    bool is_last = false;

    while (std::getline(iss, buffer, ' '))
    {
        std::string split = buffer;
        if (is_first && !is_prefix_exist && split[0] == ':')
        {
            split.substr(1);
            cmd->addParams(split);
            is_first = false;
            is_prefix_exist = true;
        }
        else
        {
            if (!is_last && split[0] == ':')
            {
                is_last = true;
                break;
            }
            else
            {
                setCmd(split, cmd);
            }
        }
    }
    if (is_last)
    {
        std::size_t pos = 0;
        if (is_prefix_exist)
        {
            pos = split.find(':', 0);
        }
        pos = split.find(':', pos);
        std::string sub_last = split.substr(pos + 1);
        cmd->addParams(sub_last);
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
        cmd->addParams(split);
    }
}