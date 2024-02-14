#include <iostream>
#include "../Response/Response.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"

int main() {
    
    Client a;
    a.setNick("test");
    a.setUser("user");

    Response b;

    Server c;
    c.setName("ft_irc");
    c.setVersion("ft_irc-42");
    c.setCreated(time(NULL));

    std::cout << c.getName() << std::endl;

    std::cout << b.rplWelcome_001(a, c) << std::endl;
    std::cout << b.rplYourHost_002(a, c) << std::endl;
    std::cout << b.rplCreated_003(a, c) << std::endl;
    std::cout << b.rplMyInfo_004(a, c) << std::endl;
    std::cout << b.rplISupport_005(a, c) << std::endl;

    std::cout << b.rplLUserClient_251(a, c) << std::endl;
    std::cout << b.rplLUserMe_255(a, c) << std::endl;

    std::cout << b.rplMotd_372(a, c) << std::endl;
    std::cout << b.rplMotdStart_375(a, c) << std::endl;
    std::cout << b.rplEndOfMotd_376(a, c) << std::endl;


    return 0;
}