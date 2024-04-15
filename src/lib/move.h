#include "header.h"

/// <summary>
/// from -> from square
/// to -> to square
/// promotion_or_enpassant -> store either the promotion piece or boolean representing en passant
/// capture -> piece captured
/// </summary>
struct Move
{
	square_t from = INVALID_SQ;
	square_t to = INVALID_SQ;
	u8 promotion_or_enpassant = false;
	Piece capture = EMPTY;

	bool operator==(const Move& m) { return from == m.from && to == m.to && promotion_or_enpassant == m.promotion_or_enpassant && capture == m.capture; };
	bool operator!=(const Move& m) { return from != m.from || to != m.to || promotion_or_enpassant != m.promotion_or_enpassant || capture != m.capture; };
	Move() = default;
	Move(square_t _from, square_t _to, Piece _capt = EMPTY, u8 _promo = false) :from(_from), to(_to), promotion_or_enpassant(_promo), capture(_capt) {}
};

static Move NULL_MOVE;
