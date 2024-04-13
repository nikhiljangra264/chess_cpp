#include "evaluate.h"

int eval::__evaluate(const board_t& _b, COLOR side)
{
	int calc = 0;
	// material and psqt
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			// if square is not occupied or the piece is of different color
			if (!_b.is_sq_occ(i, j) || piece_color(_b.piece_at(i, j)) != side)
				continue;

			bool iswhite = side == WHITE;

			if (is_pawn(_b.piece_at(i, j)))
			{
				calc += PAWN_VALUE + ((iswhite) ? psqt_pawn_white[i][j] : psqt_pawn_black[i][j]);
			}
			else if (is_knight(_b.piece_at(i, j)))
			{
				calc += KNIGHT_VALUE + ((iswhite) ? psqt_knight_white[i][j] : psqt_knight_black[i][j]);
			}
			else if (is_bishop(_b.piece_at(i, j)))
			{
				calc += BISHOP_VALUE + ((iswhite) ? psqt_bishop_white[i][j] : psqt_bishop_black[i][j]);
			}
			else if (is_rook(_b.piece_at(i, j)))
			{
				calc += ROOK_VALUE + ((iswhite) ? psqt_rook_white[i][j] : psqt_rook_black[i][j]);
			}
			else if (is_queen(_b.piece_at(i, j)))
			{
				calc += QUEEN_VALUE + ((iswhite) ? psqt_queen_white[i][j] : psqt_queen_black[i][j]);
			}
			else if (is_king(_b.piece_at(i, j)))
			{
				// not adding king's value beacuse ultimately it will be canceled
				calc += ((iswhite) ? psqt_king_white[i][j] : psqt_king_black[i][j]);
			}
		}
	}

	return calc;
}
