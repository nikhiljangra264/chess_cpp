#ifndef __UCI_H__
#define __UCI_H__

// External
#include <thread>
#include <vector>
#include <sstream>
#include <memory>

// Internal
#include "header.h"
#include "evaluate.h"
#include "board.h"
#include "search.h"
#include "misc.h"
#include "limits.h"

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
private:
	board_t board;
	Search engine;
	std::unique_ptr<std::thread> thread;

public:
	UCI() : thread(nullptr), engine(board) {}

	void loop(int argc, const char* argv[]);        // main loop
	void position(std::istringstream& is);          // set up position on board
	void go(std::istringstream& is);                // start the engine
	limits_t parse_limits(std::istringstream& is);  // parse the limits in the command

	void stop();
	void eval();

	~UCI() {
		if (thread && thread->joinable()) {
			thread->join();
		}
	}
};


#endif // __UCI_H__