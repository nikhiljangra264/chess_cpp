#include "evaluate.h"

int Eval::evaluate_side(const board_t& _b, COLOR side)
{
	int calc = 0;
	// material and psqt
	for (u8 rank = 0; rank < TOTAL_RANK; ++rank) {
		for (u8 file = 0; file < TOTAL_FILE; ++file) {
			// if square is not occupied or the piece is of different color
			if (!_b.is_sq_occ(rank, file) || piece_color(_b.piece_at(rank, file)) != side)
				continue;
			calc += PieceValue[_b.piece_at(rank, file)] + psqt[_b.piece_at(rank, file)][rank][file];
		}
	}

	return calc;
}
