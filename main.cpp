#include "Server/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage : " << av[0] << " <port> <password>" << std::endl;
        exit(EXIT_SUCCESS);
    }

    Server server(av[2]);
    try
    {
        server.checkPortnum(av[1]);
        server.initServSock();
        server.initKqueue();
        server.handleKqueue();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        server.setErrorCode();
    }
    return server.endServ();
}