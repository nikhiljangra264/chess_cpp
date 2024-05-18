#ifndef __MOVE_H__
#define __MOVE_H__

#include "header.h"

/// <summary>
/// from -> from square
/// to -> to square
/// promotion_or_enpassant -> store either the promotion piece or boolean representing en passant
/// promotion and en passant can't occur at the same time so it utilises the space
/// capture -> piece captured
/// </summary>
struct Move {
	square_t from = INVALID_SQ;
	square_t to = INVALID_SQ;
	u8 promotion_or_enpassant = 0;
	Piece capture = Piece::EMPTY;

	constexpr Move() = default;
	constexpr Move(square_t _from, square_t _to, Piece _capt = Piece::EMPTY, u8 _promo = 0)
		: from(_from), to(_to), promotion_or_enpassant(_promo), capture(_capt) {}

	bool operator==(const Move& m) const {
		return	from == m.from && to == m.to &&
				promotion_or_enpassant == m.promotion_or_enpassant &&
				capture == m.capture;
	}
	bool operator!=(const Move& m) const {
		return !(*this == m);
	}
};

static constexpr Move NULL_MOVE{};

/// <summary>
/// 0000KQkq
/// </summary>
struct board_state_t {
	u8 rights = 15;
	square_t ep_square = INVALID_SQ;
	u8 halfmove_count = 0;

	constexpr board_state_t() = default;
	constexpr board_state_t(int _rights, square_t _ep_square = INVALID_SQ, int _halfmove_count = 0)
		: rights(_rights), ep_square(_ep_square), halfmove_count(_halfmove_count) {}

	bool operator==(const board_state_t& other) const {
		return rights == other.rights &&
			ep_square == other.ep_square;
	}
	bool operator!=(const board_state_t& other) const {
		return !(*this == other);
	}

	// KQkq
	void set_wking_castle_queen(bool can) {
		if (can) rights |= 0b0100;
		else rights &= ~0b0100;
	}
	void set_wking_castle_king(bool can) {
		if (can) rights |= 0b1000;
		else rights &= ~0b1000;
	}
	void set_bking_castle_queen(bool can) {
		if (can) rights |= 0b0001;
		else rights &= ~0b0001;
	}
	void set_bking_castle_king(bool can) {
		if (can) rights |= 0b0010;
		else rights &= ~0b0010;
	}

	bool get_wking_castle_queen() const { return rights & 0b0100; }
	bool get_wking_castle_king() const { return rights & 0b1000; }
	bool get_bking_castle_queen() const { return rights & 0b0001; }
	bool get_bking_castle_king() const { return rights & 0b0010; }

	void revoke_castling_rights(COLOR color) {
		if (color == COLOR::WHITE)
			rights &= 0b0011;
		else
			rights &= 0b1100;
	}

	void set_castling_rights(const std::string& _r) {
		rights = 0;
		if (_r.size() <= 1)
			return;
		for (char c : _r) {
			switch (c) {
			case 'K': rights |= 0b1000; break;
			case 'Q': rights |= 0b0100; break;
			case 'k': rights |= 0b0010; break;
			case 'q': rights |= 0b0001; break;
			}
		}
	}
};



#endif // __MOVE_H__