#include "Message.hpp"

static void parsingSpace(std::string &split_input, Command *cmd);
static void setCmdInfo(std::string &split, Command *cmd);

// \n 기준으로 파싱
void Message::parsingOrigin()
{
    std::size_t start;
    std::size_t pos;

    start = 0;
    while ((pos = this->m_origin.find('\n', start)) != std::string::npos)
    {
        std::string splitInput;
        if (this->m_origin[pos - 1] == '\r')
        {
            splitInput = this->m_origin.substr(start, pos - start - 1);
        }
        else
        {
            splitInput = this->m_origin.substr(start, pos - start);
        }

        Command *cmd = new Command();

        // space 기준으로 스플릿해서 넣어주기
        parsingSpace(splitInput, cmd);

        // 세팅된 Command 클래스 넣어주기
        this->m_cmds.push_back(cmd);
        start = pos + 1;
    }

    // 남은 파라미터 추가
    if (start < this->m_origin.length())
    {
        std::string last = this->m_origin.substr(start, pos - start);
        Command *cmd = new Command();
        parsingSpace(last, cmd);

        this->m_cmds.push_back(cmd);
    }
}

static void parsingSpace(std::string &split_input, Command *cmd)
{
    std::size_t start;
    std::size_t pos;

    start = 0;
    while ((pos = split_input.find(' ', start)) != std::string::npos)
    {
        std::string tmp;
        tmp = split_input.substr(start, pos - start);
        // 첫 번째 문자열이면서, :로 시작하면 prefix
        if (start == 0 && tmp[0] == ':')
        {
            std::string subTmp = tmp.substr(1, tmp.length());
            cmd->setPrefix(subTmp);
        }
        else
        {
            setCmdInfo(tmp, cmd);
        }
        start = pos + 1;
    }

    // 마지막 남은 부분
    if (start < split_input.length())
    {
        std::string last;
        last = split_input.substr(start, pos - start);
        setCmdInfo(last, cmd);
    }
}

static void setCmdInfo(std::string &split, Command *cmd)
{
    // 커맨드가 비어있으면 커맨드 먼저 채우기
    if (cmd->getCommand() == "")
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
            std::string subTmp = split.substr(1, split.length());
            cmd->addParams(subTmp);
        }
        else
        {
            cmd->addParams(split);
        }
    }
}