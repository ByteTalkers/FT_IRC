#include "utils.hpp"

const std::string timeToString(time_t time)
{
    char buffer[80];
    struct tm *timeinfo;
    timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), "%H:%M:%S %b %d %Y", timeinfo);

    return std::string(buffer);
}

std::string intToString(int value)
{
    std::ostringstream os;
    os << value;
    return os.str();
}