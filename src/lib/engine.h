#pragma once

// #include <utility>
#include <string>
#include <algorithm>
#include <chrono>

#include "board.h"
#include "evaluate.h"

class Engine
{
private:
	board_t& board;
	Move best_move;
	u32 nodes = 0;
	std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
	u16 max_depth = MAX_DEPTH;
	u32 max_nodes = MAX_NODES;
	time_type max_time = MAX_TIME;
	bool stop = false;

	Engine(board_t& _board) :board(_board) {}

	int absearch(int alpha, int beta, u8 depth);
	void search();

	std::string to_uci(Move& m);

	bool is_move_legal(Move& m, COLOR move_maker, COLOR opposite);

	time_type time() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
	}

	bool is_time_up() const
	{
		return  time()  >= max_time;
	}
/*
	void print_stats()
	{
		std::cout << "info";
		std::cout << " depth " << max_depth;
		std::cout << " nodes " << nodes;
		std::cout << " nps " << (nodes / time());
		std::cout << " time " << time() << "ms";
		std::cout << " pv " << to_uci(best_move);
		std::cout << std::endl;
		std::cout.flush();
	}
*/
	void reset() {
		best_move = Move();
		max_depth = MAX_DEPTH;
		max_nodes = MAX_NODES;
		max_time = MAX_TIME;
	}
};
