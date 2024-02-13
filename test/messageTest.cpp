#include <iostream>
#include <string>
#include <vector>
#include "../Message/Message.hpp"

const std::string test1 = "CAP LS\r\n";
const std::string test2 = "NICK test\r\n";
const std::string test3 = "USER root root 127.0.0.1 :root\r\n";
const std::string test4 = "MODE test +i\r\n";
const std::string test5 = "PING irc.local\r\n";
const std::string test6 = "JOIN #a\r\n";
const std::string test7 = "MODE #a b\r\n";

const std::string Invalid1 = "JOIN #a";
const std::string Invalid2 = "PING #a\r";
const std::string Invalid3 = "MODE #a\n";

std::vector<const std::string> test = {test1, test2, test3, test4, test5, test6, test7};

int main()
{
    for (int i = 0; i < test.size(); i++) {
        
    }

}