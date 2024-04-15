#include "header.h"

/// <summary>
/// from -> from square
/// to -> to square
/// promotion_or_enpassant -> store either the promotion piece or boolean representing en passant
/// promotion and en passant can't occur at the same time so it utilises the space
/// capture -> piece captured
/// </summary>
struct Move
{
	square_t from = INVALID_SQ;
	square_t to = INVALID_SQ;
	u8 promotion_or_enpassant = 0;
	Piece capture = EMPTY;

	bool operator==(const Move& m) const { return from == m.from && to == m.to && promotion_or_enpassant == m.promotion_or_enpassant && capture == m.capture; };
	bool operator!=(const Move& m) const { return from != m.from || to != m.to || promotion_or_enpassant != m.promotion_or_enpassant || capture != m.capture; };
	constexpr Move() = default;
	constexpr Move(square_t _from, square_t _to, Piece _capt = EMPTY, u8 _promo = 0) :from(_from), to(_to), promotion_or_enpassant(_promo), capture(_capt) {}
};

static Move NULL_MOVE;
