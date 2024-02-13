#include "Server/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage : " << av[0] << " <port> <password>" << std::endl;
        exit(EXIT_SUCCESS);
    }
    try
    {
        Server server(av[1], av[2]);
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}