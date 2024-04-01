#include "Message.hpp"

static void parsingSpace(std::string &split, Command *cmd);
static void setCmd(std::string &split, Command *cmd);

/**
 * @brief Parses the origin of the message.
 *
 * This function parses the origin of the message by splitting it into separate lines and removing any trailing carriage
 * return characters. It then calls the parsingSpace function to further parse each line and store the resulting Command
 * objects in the m_cmds vector.
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

/**
 * @brief Parses the given string by splitting it at spaces and updates the Command object.
 *
 * This function takes a string and splits it at spaces. The resulting substrings are then used to update the Command
 * object.
 *
 * @param split The string to be parsed.
 * @param cmd Pointer to the Command object to be updated.
 */
static void parsingSpace(std::string &split, Command *cmd)
{
    std::istringstream iss(split);
    std::string buffer;
    std::string last;
    bool is_prefix_exist = false;
    bool is_last = false;
    size_t idx = 0;

    while (std::getline(iss, buffer, ' '))
    {
        std::string split = buffer;
        if (idx == 0 && split[0] == ':')
        {
            split.substr(1);
            cmd->addParams(split);
            is_prefix_exist = true;
        }
        else
        {
            if (split[0] == ':')
            {
                is_last = true;
                break;
            }
            else
            {
                setCmd(split, cmd);
            }
        }
        idx++;
    }

    if (is_last)
    {
        size_t pos = 0;
        if (is_prefix_exist)
        {
            pos = split.find(':', 0);
        }
        pos = split.find(':', pos);
        std::string sub_last = split.substr(pos + 1);
        cmd->addParams(sub_last);
    }
}

/**
 * @brief Sets the command for the given split string.
 *
 * This function sets the command for the given split string by modifying the provided `cmd` object.
 *
 * @param split The split string containing the command.
 * @param cmd A pointer to the Command object to be modified.
 */
static void setCmd(std::string &split, Command *cmd)
{
    // 커맨드가 비어있으면 커맨드 먼저 채우기
    if (cmd->getCommand().empty())
    {
        for (size_t i = 0; i < split.length(); i++)
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