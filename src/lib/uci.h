#pragma once

// ok

// External
#include <thread>
#include <vector>

// Internal
#include "header.h"
#include "evaluate.h"
#include "board.h"
#include "engine.h"

/// <summary>
/// implementation of UCI protocols
/// handle the engineimplement UCI protocols
/// currently supported commands are
/// uci->display information about engine
/// stop, quit, isready->does what you think
/// print->print the board
/// position->set the current position in the board syntax position[startpos | fen] moves <moves1> <moves2> ...
/// eval -> return evaluation score of current board
/// go->start the engine to caculate best move
/// </summary>
class UCI {
private:
    board_t board;
    Engine engine;
    std::thread* thread;

public:
    UCI() : thread(nullptr), engine(board) {}

    void output(const std::string& s) {
        std::cout << s << std::endl;
        std::cout.flush();
    }

    void stop();
    void uci();
    void isready();
    void ucinewgame();

    void eval();

    void processCommand(const std::string& input);

    ~UCI() {
        if (thread && thread->joinable()) {
            thread->join();
            delete thread;
        }
    }
};
