#pragma once

#include <array>
#include <deque>
#include <string>
#include <iostream>

#include "header.h"
#include "move.h"
#include "hashing.h"

/// <summary>
/// 0000KQkq
/// </summary>
struct board_state_t
{
	u8 rights = 15;
	square_t ep_square = INVALID_SQ;
	u8 halfmove_count = 0;

	board_state_t() = default;
	board_state_t(int _rights, square_t _ep_square=INVALID_SQ, int _halfmove_count=0) :rights(_rights),ep_square(_ep_square),halfmove_count(_halfmove_count) {}

	bool operator==(const board_state_t& other) const { return rights == other.rights && 
		ep_square == other.ep_square && 
		halfmove_count == other.halfmove_count; }
	bool operator!=(const board_state_t& other) const { return rights != other.rights || 
		ep_square != other.ep_square ||
		halfmove_count != other.halfmove_count; }

	void set_wking_castle_queen(bool can) { 
		if (can) rights = rights | static_cast<u8>(4); 
		else rights = rights &	static_cast<u8>(11); 
	}
	void set_wking_castle_king(bool can) { 
		if (can) rights = rights | static_cast<u8>(8); 
		else rights = rights &	static_cast<u8>(7); 
	}
	void set_bking_castle_queen(bool can) { 
		if (can) rights = rights | static_cast<u8>(1); 
		else rights = rights &	static_cast<u8>(14); 
	}
	void set_bking_castle_king(bool can) { 
		if (can) rights = rights | static_cast<u8>(2); 
		else rights = rights &	static_cast<u8>(13); 
	}

	bool get_wking_castle_queen() const { return rights &	static_cast<u8>(4); }
	bool get_wking_castle_king() const { return rights &	static_cast<u8>(8); }
	bool get_bking_castle_queen() const { return rights &	static_cast<u8>(1); }
	bool get_bking_castle_king() const { return rights &	static_cast<u8>(2); }

	void revoke_castling_rights(COLOR color) {
		if (color == WHITE)
			rights = rights & static_cast<u8>(3);
		else
			rights = rights & static_cast<u8>(12);
	}

	void set_castling_rights(std::string& _r) {
		rights = 0;
		if(_r.size() <= 1)
			return;

		rights = ((_r[0]=='K')?static_cast<u8>(8):0) | 
			((_r[1]=='Q')?static_cast<u8>(4):0) | 
			((_r[2]=='k')?static_cast<u8>(2):0) | 
			((_r[3]=='q')?static_cast<u8>(1):0);
	}
};

class board_t
{
protected:
	std::array<std::array<Piece, 8>, 8> board;
	COLOR turn = WHITE;
	square_t white_king{ 0,4 };
	square_t black_king{ 7,4 };
	hash_t key=0;

public:
	board_state_t board_state;

	// constructors
	board_t() : board(STARTING_POSITION) { init_key(); }
	board_t(const board_t& other):
		board(other.board),
		turn(other.turn),
		board_state(other.board_state),
		white_king(other.white_king),
		black_king(other.black_king),
		key(other.key)
	{}

	board_t(const board_t& other, Move& move) :board_t(other) { this->make_move(move); }
	board_t(board_t&& other) noexcept;

	// move assignment operator
	void operator=(board_t&& other) noexcept;

	// board information
	bool is_sq_occ(square_t& sq) const { return board[sq.rank][sq.file] != EMPTY; }
	bool is_sq_occ(u8 rank, u8 file) const { return board[rank][file] != EMPTY; }
	Piece piece_at(square_t& sq) const { return board[sq.rank][sq.file]; }
	Piece piece_at(u8 rank, u8 file) const { return board[rank][file]; }
	COLOR side_to_move() const { return turn; }
	square_t get_king_sq(COLOR color) const { return (color == WHITE) ? white_king : black_king; }

	// make or unmake move
	void make_move(Move& m);
	void make_null_move() {
		turn = static_cast<COLOR>(-turn);
		board_state.ep_square = INVALID_SQ;
	}
	void unmake_move(Move& m, board_state_t& prev_state);

	// generate moves
	std::deque<Move> get_psuedo_legal_moves();
	std::deque<Move> get_psuedo_legal_move_pawn(square_t sq);
	std::deque<Move> get_psuedo_legal_move_knight(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_bishop(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_rook(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_queen(square_t sq);
	std::deque<Move> get_psuedo_legal_move_king(square_t sq) const;
	std::deque<square_t> sq_attacked_by(square_t sq, COLOR color) const;
	bool is_sq_attacked_by(square_t sq, COLOR color) const;

	// initialize board
	void reset() {
		board = EMPTY_POSITION;
		key = 0;
	}
	void init_startpos();
	void init_fen(std::deque<std::string>& command);
	void fen_to_board(std::string& fen);
	square_t uci_to_sq(std::string& move) { return (move.size()!=2)?INVALID_SQ:square_t(move[1] - '1', move[0] - 'a'); }
	Move uci_to_move(const std::string& move) const;

	// operators
	bool operator==(const board_t& other) const {
		return board == other.board &&
			board_state == other.board_state
			&& turn == other.turn;
	}
	friend std::ostream& operator << (std::ostream& out, const board_t& _b);

	// hashing
	void init_key();
	hash_t get_key() { return key; }

};