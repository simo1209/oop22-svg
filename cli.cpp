#include <iostream>
#include <functional>
#include <map>

void open()
{
    std::string location;
    std::cin >> location;
    std::cout << "did open with params " << location << "\n";
}

std::map<std::string, std::function<void()>> cliCommands{
    {"open", open}};

int main(int argc, char const *argv[])
{

    

    while (1)
    {
        std::string command;

        try
        {
            std::cout << "Enter command: ";
            std::cin >> command;
            cliCommands.at(command)();
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Invalid command \"" << command << "\" entered.\n";
        }
    }

    return 0;
}
