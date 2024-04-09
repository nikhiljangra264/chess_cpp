#pragma once

#include <array>
#include <deque>
#include <string>
#include <iostream>

#include "header.h"

struct Move
{
	square_t from = INVALID_SQ;
	square_t to = INVALID_SQ;
	u8 promotion_or_enpassant = false;
	Piece capture = EMPTY;

	Move() = default;
	Move(square_t _from, square_t _to, Piece _capt = Piece::EMPTY, u8 _promo = false) :from(_from), to(_to), promotion_or_enpassant(_promo), capture(_capt) {}
};

/// <summary>
/// 0000kqKQ
/// </summary>
struct castling_rights
{
	u8 rights;

	castling_rights() :rights(15) {}
	castling_rights(int _rights) :rights(_rights) {}

	void set_wking_castle_queen(bool can) { if (can) rights = rights | 1; else rights = rights & 14; }
	void set_wking_castle_king(bool can) { if (can) rights = rights | 2; else rights = rights & 13; }
	void set_bking_castle_queen(bool can) { if (can) rights = rights | 4; else rights = rights & 11; }
	void set_bking_castle_king(bool can) { if (can) rights = rights | 8; else rights = rights & 7; }

	bool get_wking_castle_queen(bool can) const { return rights & 1; }
	bool get_wking_castle_king(bool can) const { return rights & 2; }
	bool get_bking_castle_queen(bool can) const { return rights & 4; }
	bool get_bking_castle_king(bool can) const { return rights & 8; }

	void revoke_castling_rights(COLOR color) {
		if (color == WHITE)
			rights = rights & 12;
		else
			rights = rights & 3;
	}
};

class board_t
{
protected:
	std::array<std::array<Piece, 8>, 8> board;
	COLOR turn = WHITE;
	castling_rights rights;
	square_t ep_square = INVALID_SQ;
	square_t white_king{ 0,4 };
	square_t black_king{ 7,4 };

public:
	board_t() : board(STARTING_POSITION) {}
	board_t(const board_t& other) {
		board = other.board;
		turn = other.turn;
		rights = other.rights;
		ep_square = other.ep_square;
		white_king = other.white_king;
		black_king = other.black_king;
	}
	board_t(const board_t& other, Move& move) :board_t(other) { this->make_move(move); }
	board_t(board_t&& other);

	void operator=(board_t&& other) noexcept;

	bool is_sq_occ(square_t sq) const { return board[sq.rank][sq.file] != EMPTY; }
	bool is_sq_occ(u8 rank, u8 file) const { return board[rank][file] != EMPTY; }
	Piece piece_at(square_t sq) const { return board[sq.rank][sq.file]; }
	Piece piece_at(u8 rank, u8 file) const { return board[rank][file]; }
	COLOR side_to_move() const { return turn; }

	castling_rights get_castling_rights() { return rights; }
	void set_castling_rights(castling_rights _rights) { rights = _rights; }

	square_t get_king_sq(COLOR color) const { return (color == WHITE) ? white_king : black_king; }

	// untested
	void make_move(Move& m);
	void make_null_move() {
		turn = COLOR(turn * -1);
		ep_square = INVALID_SQ;
	}
	void unmake_move(Move& m);
	// end untested

	std::deque<Move> get_psuedo_legal_move_pawn(square_t sq);
	std::deque<Move> get_psuedo_legal_move_knight(square_t sq);
	std::deque<Move> get_psuedo_legal_move_bishop(square_t sq);
	std::deque<Move> get_psuedo_legal_move_rook(square_t sq);
	std::deque<Move> get_psuedo_legal_move_queen(square_t sq);
	std::deque<Move> get_psuedo_legal_move_king(square_t sq);

	bool is_sq_attacked_by(square_t sq, COLOR color);
	std::deque<square_t> sq_attacked_by(square_t sq, COLOR color);

	std::deque<Move> get_psuedo_legal_moves();

	void init_startpos();
	void init_fen(std::deque<std::string>& command);
	void fen_to_board(std::string& fen);
	square_t uci_to_sq(std::string& move) { return square_t(move[1] - '0', move[0] - 'a'); }

	Move uci_to_move(const std::string& move);

	friend std::ostream& operator << (std::ostream& out, const board_t& _b);
};