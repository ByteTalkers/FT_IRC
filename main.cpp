#include <iostream>

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage : " << av[0] << " <port> <password>" << std::endl;
        exit(EXIT_SUCCESS);
    }

    return 1;
}