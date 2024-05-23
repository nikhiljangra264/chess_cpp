#ifndef EVALUATE_H
#define EVALUATE_H

#include "tt.h"
#include "move.h"
#include "board.h"
#include "psqt.h"

namespace Eval {
	int evaluate_side(const board_t& _b, COLOR side);

	inline int score_q_move(const board_t& board, const Move& move) {
		return (move.capture != EMPTY) ? mvvlva[move.capture % 6][board.piece_at(move.from) % 6] : 0;
	}
	inline int score_move(const board_t& board, const Move& move, const Move& tt_move) {
		if (move == tt_move)
			return 1000000;
		else if (move.capture != EMPTY)
			return 32000 + mvvlva[move.capture % 6][board.piece_at(move.from) % 6];
		return TT::h_table[(board.side_to_move() == WHITE) ? 0 : 1][move.from.rank][move.from.file][move.to.rank][move.to.file];
	}

	inline int evaluate(const board_t& _b)
	{
		return evaluate_side(_b, WHITE) - evaluate_side(_b, BLACK);
	}
} // namespace Eval


#endif // EVALUATE_H