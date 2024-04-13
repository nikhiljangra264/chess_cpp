#pragma once

#include "board.h"
#include "psqt.h"

class eval
{
	static int __evaluate(const board_t& _b, COLOR side);

public:
	static int score_move(Move& move)
	{
		return move.capture + move.promotion_or_enpassant;
	}
	
	static int evaluate(const board_t& _b)
	{
		return __evaluate(_b, WHITE) - __evaluate(_b, BLACK);
	}
};