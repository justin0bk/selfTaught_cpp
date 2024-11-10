#include "../src/Game.h"
#include <filesystem>
#include <iostream>

int main()
{
    // default working directory:
    std::cout << "Default working directory: " << std::filesystem::current_path() << std::endl;
    
    // change working directory to the project root
    std::filesystem::current_path("C:/Users/justi/OneDrive/My Work/Study/CPP/assignment_2");
    std::cout << "Root working directory: " << std::filesystem::current_path() << std::endl;

    Game g("C:/Users/justi/OneDrive/My Work/Study/CPP/assignment_2/src/config.txt");
    g.run();
}