#include <iostream>
#include "../Response/Response.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"

int main() {
    
    Client a;
    a.setNick("test");
    a.setUser("user");

    Server c;
    c.setName("ft_irc");
    c.setCreated(time(NULL));

    std::cout << c.getName() << std::endl;

    std::cout << Response::rplWelcome_001(a.getNick(), a.getUser()) << std::endl;
    std::cout << Response::rplYourHost_002(a.getNick(), a.getUser()) << std::endl;
    std::cout << Response::rplCreated_003(a.getNick(), c.getCreated()) << std::endl;
    std::cout << Response::rplMyInfo_004(a.getNick(), c.getName()) << std::endl;
    std::cout << Response::rplISupport_005(a.getNick()) << std::endl;

    std::cout << Response::rplLUserClient_251(a.getNick(), c.getClientCount()) << std::endl;
    std::cout << Response::rplLUserMe_255(a.getNick(), c.getClientCount()) << std::endl;

    std::cout << Response::rplMotd_372(a.getNick()) << std::endl;
    std::cout << Response::rplMotdStart_375(a.getNick(), c.getName()) << std::endl;
    std::cout << Response::rplEndOfMotd_376(a.getNick()) << std::endl;


    return 0;
}