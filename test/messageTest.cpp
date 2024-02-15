#include "../Message/Message.hpp"
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::string test1 = "CAP LS\r\n";
    std::string test2 = "NICK test\r\n";
    std::string test3 = "USER root root 127.0.0.1 :root\r\n";
    std::string test4 = "MODE test +i\r\n";
    std::string test5 = "PING irc.local\r\n";
    std::string test6 = "JOIN #a\r\n";
    std::string test7 = "MODE #a b\r\n";
    std::string test8 = "QUIT hihi\r\n";

    std::string invalid1 = "JOIN #a";
    std::string invalid2 = "PING #a\r";
    std::string invalid3 = "MODE #a\n";

    std::vector<std::string> test;
    std::vector<std::string> invalid;

    test.push_back(test1);
    test.push_back(test2);
    test.push_back(test3);
    test.push_back(test4);
    test.push_back(test5);
    test.push_back(test6);
    test.push_back(test7);
    test.push_back(test8);

    invalid.push_back(invalid1);
    invalid.push_back(invalid2);
    invalid.push_back(invalid3);

    for (int i = 0; i < test.size(); i++)
    {
        Message tmp(test[i]);
        tmp.seperateOrigin();
        tmp.display();
    }

    for (int i = 0; i < invalid.size(); i++)
    {
        Message tmp(invalid[i]);
        tmp.seperateOrigin();
        tmp.display();
    }

    return 0;
}