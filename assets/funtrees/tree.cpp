#include <iostream>
#include <string>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    std::cout << std::string(n, ' ') << "*" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << std::string(n - i, ' ')
                  << std::string(2 * i + 1, '0')
                  << std::endl;
    }
    return 0;
}
