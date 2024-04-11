#pragma once

#include "board.h"
#include "psqt.h"

class eval
{
	static int __evaluate(const board_t& _b, COLOR side);

public:
	static int score_move(board_t& board, Move& move);
	
	static int evaluate(const board_t& _b)
	{
		return __evaluate(_b, WHITE) - __evaluate(_b, BLACK);
	}
};