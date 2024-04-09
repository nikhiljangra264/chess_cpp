#include <thread>
#include <deque>
#include <string>
#include "uci.h"

void UCI::stop() {
    // stop engine and join thread
    engine.stop = true;
    if (thread != nullptr && thread->joinable()) {
        thread->join();
    }
}

void UCI::uci() {
    output("id name chess_cpp");
    output("id author Nikhil");
    output("");
    //output("option name Move Overhead type spin default 5 min 0 max 5000");
    //output("option name Ponder type check default false");
    output("uciok");
}

void UCI::isready() {
    output("readyok");
}

void UCI::ucinewgame() {
    // Do something for new game
}

void UCI::eval() {
    std::string e = std::to_string(eval::evaluate(board));
    output(e);
}


void UCI::processCommand(const std::string& input) {

    if (input.size() == 0) return;

    std::deque<std::string> command(1, std::string());

    // split the command
    for (int j = 0; j < input.size(); j++)
    {
        if (input[j] == ' ') command.push_back(std::string());
        else command.back().push_back(input[j]);
    }

    if (command[0] == "quit") {
        stop();
    }
    else if (command[0] == "stop") {
        stop();
        engine.reset();
    }
    else if (command[0] == "ucinewgame") {
        ucinewgame();
    }
    else if (command[0] == "uci") {
        uci();
    }
    else if (command[0] == "isready") {
        isready();
    }
    else if (command[0] == "setoption") {
        // Implement setoption
    }
    else if (command[0] == "position") {
        board.init_fen(command);
    }
    else if (command[0] == "print") {
        std::cout << board << std::endl;
    }
    else if (command[0] == "go") {

        if (thread && thread->joinable()) {
            thread->join();
            delete thread; // Clean up the previous thread
        }
        // Create a new thread and execute it
        thread = new std::thread([this]() { engine.search(); });
    }
    else if (command[0] == "eval") {
        eval();
    }
}