#include "board.h"
#include "hashing.h"
#include "move.h"
#include "tt.h"
#include "misc.h"
#include <sstream>

void board_t::push(const Move& m)
{
	// check for null move
	if (m == NULL_MOVE)
	{
		make_null_move();
		return;
	}

	Piece moving_piece = piece_at(m.from);

	// push to the history
	history.push(m, board_state, key);
	
	// en passant key
	if (board_state.ep_square != INVALID_SQ)
		key ^= Zobrist::en_passant_keys[(board_state.ep_square.rank == 2) ? 0 : 1][board_state.ep_square.file];
	// reset en passant
	board_state.ep_square = INVALID_SQ;

	// reset halfmove count if irreversible move
	if (m.capture != EMPTY || m.promotion_or_enpassant)
		board_state.halfmove_count = 0;

	// king moves
	if (is_king(moving_piece))
	{
		// update position
		if (turn == WHITE) white_king = m.to;
		else black_king = m.to;

		// update hash with previous castling rights
		key ^= Zobrist::castling_rights_keys[board_state.rights];

		// revoke rights
		board_state.revoke_castling_rights(turn);

		// if king is jumping more than one square than castling
		if (m.from.file == 4 && m.to.file == 2)		// queen side castling
		{
			// update hash for rook move
			key ^= Zobrist::piece_keys[board[m.to.rank][0]][m.to.rank][0];
			key ^= Zobrist::piece_keys[board[m.to.rank][0]][m.to.rank][3];

			board[m.to.rank][3] = board[m.to.rank][0];
			board[m.to.rank][0] = EMPTY;
			board_state.halfmove_count = 0;
		}
		if (m.from.file == 4 && m.to.file == 6)		// king side castling
		{
			key ^= Zobrist::piece_keys[board[m.to.rank][7]][m.to.rank][7];
			key ^= Zobrist::piece_keys[board[m.to.rank][7]][m.to.rank][5];

			board[m.to.rank][5] = board[m.to.rank][7];
			board[m.to.rank][7] = EMPTY;
			board_state.halfmove_count = 0;
		}
		board[m.to.rank][m.to.file] = moving_piece;

		// update hash
		key ^= Zobrist::castling_rights_keys[board_state.rights];
		if(m.capture != EMPTY)
			key ^= Zobrist::piece_keys[m.capture][m.to.rank][m.to.file];
		key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
	}

	// pawn moves
	else if (is_pawn(moving_piece))
	{
		board_state.halfmove_count = 0;

		// en passant
		if (m.from.file != m.to.file && !is_sq_occ(m.to))
		{
			board[m.from.rank][m.to.file] = EMPTY;
			board[m.to.rank][m.to.file] = moving_piece;

			key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
			key ^= Zobrist::piece_keys[m.capture][m.from.rank][m.to.file];
		}
		// double pawn push
		else if (m.to.rank == m.from.rank + 2 * turn)
		{
			board_state.ep_square = { m.from.rank + turn, m.from.file };
			board[m.to.rank][m.to.file] = moving_piece;

			key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
		}
		// promotion
		else if (m.to.rank == TOTAL_RANK - 1 || m.to.rank == 0)
		{
			board[m.to.rank][m.to.file] = static_cast<Piece>(m.promotion_or_enpassant);

			if(m.capture != EMPTY)
				key ^= Zobrist::piece_keys[m.capture][m.to.rank][m.to.file];
			key ^= Zobrist::piece_keys[board[m.to.rank][m.to.file]][m.to.rank][m.to.file];
		}
		else
		{
			board[m.to.rank][m.to.file] = moving_piece;

			if(m.capture != EMPTY)
				key ^= Zobrist::piece_keys[m.capture][m.to.rank][m.to.file];
			key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
		}
	}
	// rook moves revokes the castling rights
	else if (is_rook(moving_piece))
	{
		// update castling rights
		if (board_state.rights)
		{
			key ^= Zobrist::castling_rights_keys[board_state.rights];
			if (turn == WHITE)
			{
				if (m.from.file == 0)				board_state.set_wking_castle_queen(false);
				else if (m.from.file == TOTAL_FILE)	board_state.set_wking_castle_king(false);
			}
			else
			{
				if (m.from.file == 0)				board_state.set_bking_castle_queen(false);
				else if (m.from.file == TOTAL_FILE)	board_state.set_bking_castle_king(false);
			}
			key ^= Zobrist::castling_rights_keys[board_state.rights];
		}
		board[m.to.rank][m.to.file] = moving_piece;

		if(m.capture != EMPTY)
			key ^= Zobrist::piece_keys[m.capture][m.to.rank][m.to.file];
		key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
	}
	else
	{
		board[m.to.rank][m.to.file] = moving_piece;

		if(m.capture != EMPTY)
			key ^= Zobrist::piece_keys[m.capture][m.to.rank][m.to.file];
		key ^= Zobrist::piece_keys[moving_piece][m.to.rank][m.to.file];
	}

	// change turn and update halfmove count
	turn = static_cast<COLOR>(-turn);
	board_state.halfmove_count++;
	key ^= Zobrist:: side_to_move_key;

	board[m.from.rank][m.from.file] = EMPTY;
	// update hash for from square
	key ^= Zobrist::piece_keys[moving_piece][m.from.rank][m.from.file];

	// update hash for en passant
	if (board_state.ep_square != INVALID_SQ)
		key ^= Zobrist::en_passant_keys[(board_state.ep_square.rank == 2) ? 0 : 1][board_state.ep_square.file];
}

void board_t::pop()
{
	if (history.empty())
		return;

	// the turn variable is updated
	turn = static_cast<COLOR>(-turn);

	Move m = history.get_last_move();
	Piece moving_piece = piece_at(m.to);

	// king moves
	if (is_king(moving_piece))
	{
		// update position
		if (turn == WHITE) white_king = m.from;
		else black_king = m.from;

		// if king is jumping more than one square than castling
		if (m.from.file == 4 && m.to.file == 2)		// queen side castling
		{
			board[m.to.rank][0] = board[m.to.rank][3];
			board[m.to.rank][3] = EMPTY;
		}
		if (m.from.file == 4 && m.to.file == 6)		// king side castling
		{
			board[m.to.rank][7] = board[m.to.rank][5];
			board[m.to.rank][5] = EMPTY;
		}
		board[m.to.rank][m.to.file] = m.capture;
		board[m.from.rank][m.from.file] = moving_piece;
	}

	// pawn moves (no promotion moves)
	else if (is_pawn(moving_piece))
	{
		// en passant if en passant variable is true
		if (m.promotion_or_enpassant)
		{
			// place catured pawn
			board[m.from.rank][m.to.file] = m.capture;
			// update the caturing pawn
			board[m.to.rank][m.to.file] = EMPTY;
		}
		else
		{
			board[m.to.rank][m.to.file] = m.capture;
		}
		board[m.from.rank][m.from.file] = moving_piece;
	}

	// check for promotion moves
	else if ((m.to.rank == TOTAL_RANK-1 || m.to.rank == 0) && m.promotion_or_enpassant)
	{
		board[m.from.rank][m.from.file] = (turn == WHITE) ? W_PAWN : B_PAWN;
		board[m.to.rank][m.to.file] = m.capture;
	}
	// other moves
	else
	{
		board[m.to.rank][m.to.file] = m.capture;
		board[m.from.rank][m.from.file] = moving_piece;
	}
	
	board_state = history.get_last_board_state();
	key = history.get_last_key();
	// pop
	history.pop();
}

MOVES board_t::get_psuedo_legal_move_pawn(square_t sq)
{
	MOVES moves;
	COLOR color = piece_color(piece_at(sq));
	square_t to{sq.rank, sq.file};

	// promotion
	if ((color == WHITE && sq.rank == 6) || (color == BLACK && sq.rank == 1))
	{
		to.rank += color;
		// pawn push
		if (!is_sq_occ(to))
		{
			moves.push_back(Move(sq, to, EMPTY, (color == WHITE) ? W_KNIGHT : B_KNIGHT));
			moves.push_back(Move(sq, to, EMPTY, (color == WHITE) ? W_QUEEN : B_QUEEN));
		}
		// attack
		to.file += 1;
		// if the attacking square has opposite color piece
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) != color)
		{
			moves.push_back(Move(sq, to, piece_at(to), (color == WHITE) ? W_KNIGHT : B_KNIGHT));
			moves.push_back(Move(sq, to, piece_at(to), (color == WHITE) ? W_QUEEN : B_QUEEN));
		}
		to.file -= 2;
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) != color)
		{
			moves.push_back(Move(sq, to, piece_at(to), (color == WHITE) ? W_KNIGHT : B_KNIGHT));
			moves.push_back(Move(sq, to, piece_at(to), (color == WHITE) ? W_QUEEN : B_QUEEN));
		}
	}

	else
	{
		to.rank += color;
		// pawn push
		if (!is_sq_occ(to))
		{
			moves.push_back(Move(sq, to));
			to.rank += color;
			if (((color == WHITE && sq.rank == 1) || (color == BLACK && sq.rank == 6)) && !is_sq_occ(to))
				moves.push_back(Move(sq, to));
		}

		// attack
		to = {sq.rank + color, sq.file + 1};
		// if the attacking square has opposite color piece
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) != color)
			moves.push_back(Move(sq, to, piece_at(to)));

		to.file -= 2;
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) != color)
			moves.push_back(Move(sq, to, piece_at(to)));

		// en passant
		if (board_state.ep_square != INVALID_SQ &&
			board_state.ep_square.rank == sq.rank + color &&
			(board_state.ep_square.file == sq.file - 1 || board_state.ep_square.file == sq.file + 1))
			moves.push_back(Move(sq, board_state.ep_square, piece_at(sq.rank, board_state.ep_square.file), true));
	}

	return moves;
}

MOVES board_t::get_psuedo_legal_move_knight(square_t sq) const
{
	MOVES moves;
	COLOR color = piece_color(piece_at(sq));
	for (auto &dir : KNIGHT_DIRECTIONS)
	{
		square_t to{sq.rank + dir[0], sq.file + dir[1]};
		// if square is occ by same color piece we can't move to the sq so skip current move
		if (!to.inbound() || (is_sq_occ(to) && piece_color(piece_at(to)) == color))
			continue;

		moves.push_back(Move(sq, to, piece_at(to)));
	}
	return moves;
}

MOVES board_t::get_psuedo_legal_move_bishop(square_t sq) const
{
	MOVES moves;
	COLOR color = piece_color(piece_at(sq));
	// for every direction
	for (auto &dir : BISHOP_DIRECTIONS)
	{
		square_t to{sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if we can move the piece
			if (is_sq_occ(to))
			{
				if (piece_color(piece_at(to)) != color)
					moves.push_back(Move(sq, to, piece_at(to)));
				break;
			}
			else
			{
				moves.push_back(Move(sq, to));
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}
	return moves;
}

MOVES board_t::get_psuedo_legal_move_rook(square_t sq) const
{
	MOVES moves;
	COLOR color = piece_color(piece_at(sq));
	// for every direction
	for (auto &dir : ROOK_DIRECTIONS)
	{
		square_t to{sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if we can move the piece
			if (is_sq_occ(to))
			{
				if (piece_color(piece_at(to)) != color)
					moves.push_back(Move(sq, to, piece_at(to)));
				break;
			}
			else
			{
				moves.push_back(Move(sq, to));
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}
	return moves;
}

MOVES board_t::get_psuedo_legal_move_queen(square_t sq) const
{
	MOVES bishop_moves = std::move(get_psuedo_legal_move_bishop(sq));
	MOVES rook_moves = std::move(get_psuedo_legal_move_rook(sq));
	rook_moves.insert(rook_moves.end(), std::make_move_iterator(bishop_moves.begin()), std::make_move_iterator(bishop_moves.end()));
	return rook_moves;
}

MOVES board_t::get_psuedo_legal_move_king(square_t sq) const
{
	MOVES moves;
	COLOR color = piece_color(piece_at(sq));
	// simple moves
	for (auto &dir : ALL_DIRECTIONS)
	{
		square_t to{sq.rank + dir[0], sq.file + dir[1]};
		// if square is occ by same color piece we can't move to the sq so skip current move
		if (!to.inbound() || (is_sq_occ(to) && piece_color(piece_at(to)) == color))
			continue;

		moves.push_back(Move(sq, to, piece_at(to)));
	}
	// castling moves
	if (color == WHITE)
	{
		if (board_state.get_wking_castle_queen() && board[0][2] == EMPTY && board[0][3] == EMPTY && board[0][1] == EMPTY)
			moves.push_back(Move(sq, { 0,2 }));
		if (board_state.get_wking_castle_king() && board[0][5] == EMPTY && board[0][6] == EMPTY)
			moves.push_back(Move(sq, { 0,6 }));
	}
	else
	{
		if (board_state.get_bking_castle_queen() && board[7][2] == EMPTY && board[7][3] == EMPTY && board[7][1] == EMPTY)
			moves.push_back(Move(sq, { 7,2 }));
		if (board_state.get_bking_castle_king() && board[7][5] == EMPTY && board[7][6] == EMPTY)
			moves.push_back(Move(sq, { 7,6 }));
	}
	return moves;
}

MOVES board_t::get_psuedo_legal_moves()
{
	MOVES moves;

	for (u8 rank = 0; rank < TOTAL_RANK; ++rank)
	{
		for (u8 file = 0; file < TOTAL_FILE; ++file)
		{
			// if square is empty or different color
			if (piece_at(rank, file) == EMPTY || piece_color(piece_at(rank, file)) != turn)
				continue;
			else if (is_pawn(piece_at(rank, file)))
			{
				MOVES temp = std::move(get_psuedo_legal_move_pawn({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_knight(piece_at(rank, file)))
			{
				MOVES temp = std::move(get_psuedo_legal_move_knight({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_bishop(piece_at(rank, file)))
			{
				MOVES temp = std::move(get_psuedo_legal_move_bishop({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_rook(piece_at(rank, file)))
			{
				MOVES temp = std::move(get_psuedo_legal_move_rook({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_queen(piece_at(rank, file)))
			{
				MOVES temp = std::move(get_psuedo_legal_move_queen({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else
			{
				MOVES temp = std::move(get_psuedo_legal_move_king({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
		}
	}

	return moves;
}

bool board_t::is_sq_attacked_by(square_t sq, COLOR color) const
{
	square_t to = INVALID_SQ;

	// check for pawn
	to = {sq.rank - color, sq.file + 1};
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		return true;
	to.file -= 2;
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		return true;

	// check for king
	for (auto& dir : ALL_DIRECTIONS)
	{
		to = { sq.rank + dir[0], sq.file + dir[1] };
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_king(piece_at(to)))
			return true;
	}

	// Check for knight
	for (auto &dir : KNIGHT_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_knight(piece_at(to)))
			return true;
	}

	// Check orthogonally for queen or rook of opposite color
	for (auto &dir : ROOK_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if square is occupied
			if (is_sq_occ(to))
			{
				// if queen and rook
				if (piece_color(piece_at(to)) == color && (is_queen(piece_at(to)) || is_rook(piece_at(to))))
					return true;
				break;
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}
	// Check diagonally for queen or bihsop of opposite color
	for (auto &dir : BISHOP_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if square is occupied
			if (is_sq_occ(to))
			{
				// if bishop queen
				if (piece_color(piece_at(to)) == color && (is_queen(piece_at(to)) || is_bishop(piece_at(to))))
				{
					return true;
				}
				break;
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}

	return false;
}

std::deque<square_t> board_t::sq_attacked_by(square_t sq, COLOR color) const
{
	std::deque<square_t> squares;
	square_t to = INVALID_SQ;
	// Check orthogonally for queen or rook of opposite color
	for (auto &dir : ROOK_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if square is occupied
			if (is_sq_occ(to))
			{
				// if same color queen and rook
				if (piece_color(piece_at(to)) == color && (is_queen(piece_at(to)) || is_rook(piece_at(to))))
					squares.push_back(to);
				break;
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}
	// Check diagonally for queen or bihsop of opposite color
	for (auto &dir : BISHOP_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		// while the sq is valid
		while (to.inbound())
		{
			// check if square is occupied
			if (is_sq_occ(to))
			{
				// if different color bishop queen
				if (piece_color(piece_at(to)) == color && (is_queen(piece_at(to)) || is_bishop(piece_at(to))))
					squares.push_back(to);
				break;
			}
			to.rank += dir[0];
			to.file += dir[1];
		}
	}
	// Check for knight of opposite color
	for (auto &dir : KNIGHT_DIRECTIONS)
	{
		to = {sq.rank + dir[0], sq.file + dir[1]};
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_knight(piece_at(to)))
			squares.push_back(to);
	}
	// check for pawn
	to = {sq.rank - color, sq.file + 1};
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		squares.push_back(to);
	to.file -= 2;
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		squares.push_back(to);
	// check for king
	for (auto& dir : ALL_DIRECTIONS)
	{
		to = { sq.rank + dir[0], sq.file + dir[1] };
		if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_king(piece_at(to)))
			squares.push_back(to);
	}

	return squares;
}

void board_t::set_position(const std::string& fen, const std::vector<std::string>& moves)
{
	std::istringstream ss(fen);
	reset();

	/*
	expected sequence of arrived fen is
	board    turn    castling_rights        en passant        half_moves        full_moves
	*/
	std::string token;

	// Board setup
	if (!(ss >> token))
		throw std::invalid_argument("Invalid FEN: missing board part");
	init_fen(token);

	// Turn
	if (!(ss >> token))
	{
		LOG::log_error("Invalid FEN: missing turn part");
		turn = WHITE;
	}
	else if (token == "w")
		turn = WHITE;
	else
		turn = BLACK;

	// Castling rights
	if (!(ss >> token))
		LOG::log_error("Invalid FEN: missing castling rights part");
	board_state.set_castling_rights(token);

	// En passant
	if (!(ss >> token))
	{
		LOG::log_error("Invalid FEN: missing en passant part");
		board_state.ep_square = INVALID_SQ;
	}
	if (token != "-")
	{
		if (token.size() != 2 || token[0] < 'a' || token[0] > 'h' || token[1] < '1' || token[1] > '8')
			throw std::invalid_argument("Invalid FEN: invalid en passant square");
		board_state.ep_square.file = token[0] - 'a';
		board_state.ep_square.rank = token[1] - '1';
	}
	else
	{
		board_state.ep_square = INVALID_SQ;
	}

	// Half move count
	if (!(ss >> token))
	{
		LOG::log_error("Invalid FEN: missing half move count");
		board_state.halfmove_count = 0;
	}
	board_state.halfmove_count = std::stoi(token);

	// init key
	init_key();

	// Apply moves
	for (const std::string& move : moves)
	{
		if (move.size() < 4 || move.size() > 5)
			throw std::invalid_argument("Invalid move format: " + move);
		Move m = uci_to_move(move);
		push(m);
	}
}

void board_t::init_fen(const std::string& fen)
{
	int rank = TOTAL_RANK - 1, file = 0; // Initialize rank and file
	for (char c : fen)
	{
		if (rank < 0 || file > TOTAL_FILE) // Check if board boundaries are exceeded
			throw std::invalid_argument("Invalid FEN: Board setup exceeds boundaries");

		if (c == '/')
		{
			// Move to the next rank
			rank--;
			file = 0; // Reset file to 0 for the next rank
		}
		else if (isdigit(c))
		{
			file += c - '0';
			if (file > TOTAL_FILE)
			{
				LOG::log_error("Invalid FEN: Too many empty squares in a rank");
				file = 0;
			}
		}
		else
		{
			// Piece symbols
			Piece piece = EMPTY;
			for (int i = 0; i < 13; ++i)
			{
				if (c == PieceSymbol[i])
				{
					piece = static_cast<Piece>(i);
					break;
				}
			}
			if (piece == EMPTY)
				throw std::invalid_argument("Invalid FEN: Unknown piece symbol");

			// Place the piece on the board
			if (rank >= 0 && rank < TOTAL_RANK && file >= 0 && file < TOTAL_FILE)
			{
				board[rank][file] = piece;
				file++; // Move to the next file
			}
			else
			{
				throw std::invalid_argument("Invalid FEN: Piece placement out of board boundaries");
			}
		}
	}
	if (rank != 0 || file != TOTAL_FILE)
		throw std::invalid_argument("Invalid FEN: Incomplete board setup");
}

Move board_t::uci_to_move(const std::string& move) const
{
	Piece promotion = EMPTY;

	// Null move
	if (move == "0000" || move.size() < 4 || move.size() > 5)
		return NULL_MOVE;

	// Check for promotion
	if (move.size() == 5)
	{
		switch (move[4])
		{
		case 'q':
			promotion = (turn == WHITE) ? W_QUEEN : B_QUEEN;
			break;
		case 'n':
			promotion = (turn == WHITE) ? W_KNIGHT : B_KNIGHT;
			break;
		case 'b':
			promotion = (turn == WHITE) ? W_BISHOP : B_BISHOP;
			break;
		case 'r':
			promotion = (turn == WHITE) ? W_ROOK : B_ROOK;
			break;
		default:
			LOG::log_error("invalid promotion character");
			return NULL_MOVE;
		}
	}

	square_t from{ move[1] - '1', move[0] - 'a' };
	square_t to{ move[3] - '1', move[2] - 'a' };

	// Check if source and destination squares are within the board boundaries
	if (!from.inbound() || !to.inbound())
		return NULL_MOVE;

	// Check for en passant
	// if pawn and attacking and attack square empty
	if (is_pawn(piece_at(from)) && from.file != to.file && !is_sq_occ(to))
		return Move(from, to, piece_at(from.rank, to.file), true);

	return Move(from, to, piece_at(to), promotion);
}

void board_t::init_key()
{
	key = 0;
	// XOR all Zobrist keys for pieces on the starting board position
	// Include additional information (e.g., side to move, castling rights, en passant square)

	for (int rank = 0; rank < TOTAL_RANK; ++rank)
	{
		for (int file = 0; file < TOTAL_FILE; ++file)
		{
			if (piece_at(rank, file) != EMPTY)
			{
				key ^= Zobrist::piece_keys[piece_at(rank, file)][rank][file];
			}
		}
	}
	// side to move
	if (turn == WHITE)
		key ^= Zobrist::side_to_move_key;

	// castling rights
	key ^= Zobrist::castling_rights_keys[board_state.rights];

	// en passant move
	if (board_state.ep_square != INVALID_SQ)
	{
		if (board_state.ep_square.rank == 2)
			key ^= Zobrist::en_passant_keys[0][board_state.ep_square.file];
		else
			key ^= Zobrist::en_passant_keys[1][board_state.ep_square.file];
	}
}

std::ostream& operator<<(std::ostream& out, const board_t& _b)
{
	out << "\n";
	for (int rank = TOTAL_RANK - 1; rank >= 0; --rank)
	{
		out << rank + 1 << "\t"; // Print rank number
		for (int file = 0; file < TOTAL_FILE; ++file)
		{
			Piece piece = _b.piece_at(rank, file);
			out << PieceSymbol[piece] << ' ';
		}
		out << '\n';
	}
	out << "\n\n\ta b c d e f g h\n"; // Print file labels
	// print board stats
	out << "en passant square: ";
	if (_b.board_state.ep_square == INVALID_SQ)
		out << "-\n";
	else
	out	<< static_cast<char>(_b.board_state.ep_square.file + 'a')
		<< static_cast<char>(_b.board_state.ep_square.rank + '1') << "\n";
	out << "castling rights: "
		<< (_b.board_state.get_wking_castle_king() ? 'K' : '-')
		<< (_b.board_state.get_wking_castle_queen() ? 'Q' : '-')
		<< (_b.board_state.get_bking_castle_king() ? 'k' : '-')
		<< (_b.board_state.get_bking_castle_queen() ? 'q' : '-') << '\n';
	out << "turn: " << (_b.turn == WHITE ? 'W' : 'B') << '\n';
	return out;
}