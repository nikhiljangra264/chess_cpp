#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <unordered_map>
#include <deque>

#include "header.h"

/// <summary>
/// Represents a move in the chess game.
/// from -> from square
/// to -> to square
/// promotion_or_enpassant -> stores either the promotion piece or a boolean representing en passant
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
		return from == m.from && to == m.to &&
			promotion_or_enpassant == m.promotion_or_enpassant &&
			capture == m.capture;
	}

	bool operator!=(const Move& m) const {
		return !(*this == m);
	}

	// convert the move to UCI format
	std::string to_uci() const {
		std::string uci_format;

		// Null move
		if (from == INVALID_SQ || to == INVALID_SQ)
			return "0000";

		uci_format.push_back('a' + from.file);
		uci_format.push_back('1' + from.rank);
		uci_format.push_back('a' + to.file);
		uci_format.push_back('1' + to.rank);

		// Add promotion piece if applicable
		if ((to.rank == 0 || to.rank == 7) && promotion_or_enpassant != Piece::EMPTY) {
			if (is_knight(static_cast<Piece>(promotion_or_enpassant)))
				uci_format.push_back('n');
			else if (is_bishop(static_cast<Piece>(promotion_or_enpassant)))
				uci_format.push_back('b');
			else if (is_rook(static_cast<Piece>(promotion_or_enpassant)))
				uci_format.push_back('r');
			else if (is_queen(static_cast<Piece>(promotion_or_enpassant)))
				uci_format.push_back('q');
		}

		return uci_format;
	}
};

// Special move representing a null move
static constexpr Move NULL_MOVE{};
// array of moves
using MOVES = std::deque<Move>;

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

	void reset() {
		rights = 0;
		ep_square = INVALID_SQ;
		halfmove_count = 0;
	}
};

/// <summary>
/// stores moves history
/// required for undo moves
/// </summary>
class board_history_t
{
	MOVES move_history;
	std::deque<board_state_t> board_state_history;
	std::deque<hash_t> key_history;
	std::unordered_map<hash_t, int> key_cache;

public:
	board_history_t() = default;

	// Copy constructor
	board_history_t(const board_history_t& other)
		: move_history(other.move_history),
		board_state_history(other.board_state_history),
		key_history(other.key_history),
		key_cache(other.key_cache) {}

	// Copy assignment operator
	board_history_t& operator=(const board_history_t& other)
	{
		if (this != &other)
		{
			move_history = other.move_history;
			board_state_history = other.board_state_history;
			key_history = other.key_history;
			key_cache = other.key_cache;
		}
		return *this;
	}

	// Move constructor
	board_history_t(board_history_t&& other) noexcept
		: move_history(std::move(other.move_history)),
		board_state_history(std::move(other.board_state_history)),
		key_history(std::move(other.key_history)),
		key_cache(std::move(other.key_cache)) {}

	// Move assignment operator
	board_history_t& operator=(board_history_t&& other) noexcept
	{
		if (this != &other)
		{
			move_history = std::move(other.move_history);
			board_state_history = std::move(other.board_state_history);
			key_history = std::move(other.key_history);
			key_cache = std::move(other.key_cache);
		}
		return *this;
	}

	void push(const Move move, const board_state_t board_state, hash_t key) {
		move_history.push_back(move);
		board_state_history.push_back(board_state);
		key_history.push_back(key);
		if (key_cache.find(key) != key_cache.end())
			key_cache[key]++;
		else
			key_cache[key] = 1;
	}
	void pop() {
		if (!key_history.empty())
		{
			hash_t key = key_history.back();
			move_history.pop_back();
			board_state_history.pop_back();
			if (key_cache[key] <= 1)
				key_cache.erase(key);
			else
				key_cache[key]--;
			key_history.pop_back();
		}
	}
	bool empty() const { return move_history.empty(); }

	Move get_last_move() const
	{
		return move_history.empty() ? Move() : move_history.back();
	}

	board_state_t get_last_board_state() const
	{
		return board_state_history.empty() ? board_state_t() : board_state_history.back();
	}

	hash_t get_last_key() const 
	{
		return key_history.empty() ? 0 : key_history.back();
	}

	bool check_three_fold_repetition(hash_t key) const 
	{
		return key_cache.find(key) != key_cache.end() && key_cache.at(key) >= 3;
	}
	bool position_occured(hash_t key) const 
	{
		return key_cache.find(key) != key_cache.end();
	}

	void reset()
	{
		move_history.clear();
		board_state_history.clear();
		key_history.clear();
		key_cache.clear();
	}
};


#endif // MOVE_H