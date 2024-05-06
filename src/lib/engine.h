#pragma once

// #include <utility>
#include <string>
#include <algorithm>
#include <chrono>
#include <utility>
#include <set>
#include <unordered_map>

#include "board.h"
#include "evaluate.h"

class Engine
{
private:
	board_t& board;
	Move best_move;
	u32 nodes = 0;
	std::chrono::time_point<std::chrono::steady_clock> start_time;

	std::unordered_map<hash_t, std::pair<int, std::shared_ptr<std::deque<Move>>>> table;
	std::set<hash_t> cache_positions;

public:
	u16 max_depth = MAX_DEPTH;
	u32 max_nodes = MAX_NODES;
	time_type max_time = MAX_TIME;
	bool stop = false;
	// constructor
	Engine(board_t& _board) :board(_board) {}

	// main search algo
	void iterative_deepening();
	int absearch(int alpha, int beta, u16 depth, u16 ply);
	void search();

	// move
	std::string to_uci(Move& m);
	bool is_move_legal(Move& m);

	// time
	time_type time() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
	}
	bool is_time_up() const
	{
		return  time()  >= max_time;
	}

	// resets
	void complete_reset()
	{
		reset();
		table.clear();
		cache_positions.clear();
	}
	void reset() {
		best_move = Move();
		max_depth = MAX_DEPTH;
		max_nodes = MAX_NODES;
		max_time = MAX_TIME;
	}
};
