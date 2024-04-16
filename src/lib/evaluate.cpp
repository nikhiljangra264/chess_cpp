#include "evaluate.h"

int eval::__evaluate(const board_t& _b, COLOR side)
{
	int calc = 0;
	// material and psqt
	for (u8 i = 0; i < 8; ++i) {
		for (u8 j = 0; j < 8; ++j) {
			// if square is not occupied or the piece is of different color
			if (!_b.is_sq_occ(i, j) || piece_color(_b.piece_at(i, j)) != side)
				continue;
			calc += PieceValue[_b.piece_at(i, j)] + psqt[_b.piece_at(i, j)][i][j];
		}
	}

	return calc;
}
