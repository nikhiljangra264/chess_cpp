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
    output("option name Move Overhead type spin default 5 min 0 max 5000");
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
        zobrist_hasher::init();
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
        engine.reset();
        for (int index = 1; index < command.size(); index++)
        {
            if (command[index] == "depth")
            {
                engine.max_depth = std::stoi(command[index++ + 1]);
                if (engine.max_depth == 0) throw std::invalid_argument("depth value 0!");
            }
            else if (command[index] == "nodes")
            {
                engine.max_nodes = std::stoi(command[index++ + 1]);
                if (engine.max_nodes == 0) throw std::invalid_argument("nodes value 0!");
            }
            else if (command[index] == "movetime")
                engine.max_time = std::stoi(command[index++ + 1]);
            else if (command[index] == "wtime" && board.side_to_move() == WHITE)
                engine.max_time = std::stoi(command[index++ + 1]);
            else if (command[index] == "btime" && board.side_to_move() == BLACK)
                engine.max_time = std::stoi(command[index++ + 1]);
            else if (command[index] == "winc" && board.side_to_move() == WHITE)
                engine.max_time += std::stoi(command[index++ + 1]);
            else if (command[index] == "binc" && board.side_to_move() == BLACK)
                engine.max_time += std::stoi(command[index++ + 1]);
        }

        engine.stop = false;

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