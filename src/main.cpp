#include <iostream>
#include <map>
#include <functional>

#include "SVGProgram.hpp"

int main(int argc, char const *argv[])
{

    SVGProgram program;

    while (1)
    {
        std::string command;

        try
        {
            std::cout << "Enter command: ";
            std::cin >> command;
            program.run(command);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return 0;
}
