// chess_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// ok

#include <iostream>
#include <string>

#include "lib/uci.h"

int main(int argc, char const* argv[])
{
    // uci instance
    UCI game;
    std::string line;

    for (int i = 1; i < argc; i++) {
        line = argv[i];
        game.processCommand(line);
    }

    while (std::getline(std::cin, line))
    {
        game.processCommand(line);
        if (line == "quit")
            break;
    }
    return 0;
}
