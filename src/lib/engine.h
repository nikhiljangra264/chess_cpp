#pragma once

#include <utility>
#include <string>
#include <stack>
#include <algorithm>

#include "board.h"
#include "evaluate.h"
#include "limit.h"

class Engine
{
private:
	board_t& board;
	Move best_move;
	u16 nodes = 0;

public:
	u16 max_depth = limit.max_depth;
	u16 max_nodes = limit.max_nodes;
	bool stop = false;

	Engine(board_t& _board) :board(_board) {}

	int absearch(int alpha, int beta, u16 depth);
	void search();

	std::string to_uci(Move& m);

	void reset() {
		best_move = Move();
		max_depth = MAX_DEPTH;
		max_nodes = -1;
	}
};
