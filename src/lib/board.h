#ifndef __BOARD_H__
#define __BOARD_H__

#include <array>
#include <deque>
#include <vector>
#include <string>
#include <iostream>

#include "header.h"
#include "move.h"
#include "hashing.h"
#include "misc.h"

class board_t
{
protected:
	std::array<std::array<Piece, TOTAL_FILE>, TOTAL_RANK> board;

	// Store move history which contains move and board_state
	board_history_t history;

public:
	board_state_t board_state;

	// constructors
	board_t() { 
		reset();
		init_key();
	}

	board_t(const board_t& other)
		: board(other.board),
		key(other.key),
		turn(other.turn),
		white_king(other.white_king),
		black_king(other.black_king),
		board_state(other.board_state),
		history(other.history) {}

	board_t(const board_t& other, Move& move) :board_t(other) { push(move); }

	board_t(board_t&& other) noexcept
		: board(std::move(other.board)),
		key(other.key),
		turn(other.turn),
		white_king(other.white_king),
		black_king(other.black_king),
		board_state(other.board_state),
		history(std::move(other.history)) {}

	// Move assignment operator
	board_t& operator=(board_t&& other) noexcept {
		if (this != &other) {
			board = std::move(other.board);
			key = other.key;
			turn = other.turn;
			white_king = other.white_king;
			black_king = other.black_king;
			board_state = other.board_state;
			history = std::move(other.history);
		}
		return *this;
	}

	// board information
	bool is_sq_occ(const square_t& sq) const { return board[sq.rank][sq.file] != EMPTY; }
	bool is_sq_occ(u8 rank, u8 file) const { return board[rank][file] != EMPTY; }
	Piece piece_at(const square_t& sq) const { return board[sq.rank][sq.file]; }
	Piece piece_at(u8 rank, u8 file) const { return board[rank][file]; }
	COLOR side_to_move() const { return turn; }
	square_t get_king_sq(COLOR color) const { return (color == WHITE) ? white_king : black_king; }

	// make or unmake move
	void push(const Move& m);
	void make_null_move() {
		turn = static_cast<COLOR>(-turn);
		board_state.ep_square = INVALID_SQ;
	}
	void pop();

	// generate moves
	std::deque<Move> get_psuedo_legal_moves();
	std::deque<Move> get_psuedo_legal_move_pawn(square_t sq);
	std::deque<Move> get_psuedo_legal_move_knight(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_bishop(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_rook(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_queen(square_t sq) const;
	std::deque<Move> get_psuedo_legal_move_king(square_t sq) const;
	std::deque<square_t> sq_attacked_by(square_t sq, COLOR color) const;
	bool is_sq_attacked_by(square_t sq, COLOR color) const;

	// initialize board
	void reset() {
		for (auto& rank : board) {
			rank.fill(EMPTY);
		}
		key = 0;
	}

	void set_position(const std::string& fen, const std::vector<std::string>& moves = {});
	void init_fen(const std::string& fen);
	Move uci_to_move(const std::string& move) const;
	square_t uci_to_sq(const std::string move) const {
		return (move.size() != 2) ? INVALID_SQ : square_t(move[1] - '1', move[0] - 'a');
	}

	// Operators
	bool operator==(const board_t& other) const {
		return	board == other.board &&
			board_state == other.board_state &&
			turn == other.turn &&
			key == other.key;
	}
	friend std::ostream& operator<<(std::ostream& out, const board_t& _b);

	// Hashing
	void init_key(); // Starting initialization. Note: Updating happens while making a move
	hash_t get_key() const { return key; }

protected:
	square_t white_king{ 0, TOTAL_FILE / 2 };
	square_t black_king{ TOTAL_RANK - 1, TOTAL_FILE / 2 };
	hash_t key{};
	COLOR turn{ WHITE };
};

#endif // __BOARD_H__