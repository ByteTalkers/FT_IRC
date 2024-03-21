#include "Message.hpp"

static void parsingSpace(std::string &split, Command *cmd);
static void setCmd(std::string &split, Command *cmd);

/**
 * @brief Message 클래스의 parsingOrigin 함수입니다.
 *        주어진 문자열을 개행 문자('\n')을 기준으로 분리하여 처리합니다.
 *        분리된 문자열을 parsingSpace 함수를 통해 공백 문자(' ')을 기준으로 분리하여 Command 객체로 변환하고,
 *        변환된 Command 객체를 m_cmds 벡터에 추가합니다.
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
 * @brief 주어진 문자열을 공백을 기준으로 분리하여 처리하는 함수입니다.
 *
 * @param split 분리할 문자열
 * @param cmd Command 객체에 결과를 저장하기 위한 포인터
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
 * @brief 주어진 문자열을 사용하여 Command 객체의 커맨드와 파라미터를 설정합니다.
 *
 * @param split 문자열을 나눈 결과
 * @param cmd Command 객체의 포인터
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