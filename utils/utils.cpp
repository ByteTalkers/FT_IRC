#include "utils.hpp"

const std::string timeToString(time_t time)
{
    char buffer[80];
    struct tm* timeinfo;
    timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), "%H:%M:%S %b %d %Y" ,timeinfo);

    return std::string(buffer);
}