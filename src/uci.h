#ifndef UCI_H
#define UCI_H

// External
#include <thread>
#include <sstream>

// Internal
#include "board.h"
#include "search.h"

/// <summary>
/// Implementation of UCI protocols.
/// Handles the engine's interaction with UCI, API or USER.
/// Currently supported commands are:
///   - uci: Display information about the engine.
///   - stop, quit, isready: Perform the expected actions.
///   - position: Set the current position on the board. Syntax: position [startpos | fen] moves (moves1) (moves2) ...
///   - go: Start the engine to calculate the best move.
///   - print: Print the board (for debugging).
///   - eval: Return the evaluation score of the current board (for debugging).
/// </summary>
class UCI {
   public:
    UCI() :
        thread(nullptr) {}

    void     loop(int argc, const char* argv[]);    // main loop
    void     position(std::istringstream& is);      // set up position on board
    void     go(std::istringstream& is);            // start the engine
    limits_t parse_limits(std::istringstream& is);  // parse the limits
    void     setoptions(std::istringstream& is);    // parse the options

    void stop();
    void eval();

    ~UCI() {
        if (thread && thread->joinable())
        {
            thread->join();
        }
    }

   private:
    board_t                      board;
    Search                       engine;
    std::unique_ptr<std::thread> thread;
    std::mutex                   engine_mutex;
};


#endif  // UCI_H