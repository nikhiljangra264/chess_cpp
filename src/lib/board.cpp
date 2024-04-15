#include "board.h"

board_t::board_t(board_t &&other) noexcept
{
	board = std::move(other.board);
	turn = other.turn;
	board_state = other.board_state;
	white_king = other.white_king;
	black_king = other.black_king;
}

void board_t::operator=(board_t &&other) noexcept
{
	board = std::move(other.board);
	turn = other.turn;
	board_state = other.board_state;
	white_king = other.white_king;
	black_king = other.black_king;
}

void board_t::make_move(Move &m)
{
	Piece moving_piece = piece_at(m.from);
	// reset halfmove count if irreversible move
	if (m.capture != EMPTY || m.promotion_or_enpassant)
		board_state.halfmove_count = 0;
	// reset en passant
	board_state.ep_square = INVALID_SQ;

	// king moves
	if (is_king(moving_piece))
	{
		// update position
		if (turn == WHITE) white_king = m.to;
		else black_king = m.to;

		// revoke rights
		board_state.revoke_castling_rights(turn);

		// if king is jumping more than one square than castling
		// queen side castling
		if (m.from.file == 4 && m.to.file == 2)
		{
			board[m.to.rank][0] = EMPTY;
			board[m.to.rank][3] = (turn == WHITE) ? W_ROOK : B_ROOK;
			board_state.halfmove_count = 0;
		}
		if (m.from.file == 4 && m.to.file == 6)
		{
			board[m.to.rank][7] = EMPTY;
			board[m.to.rank][5] = (turn == WHITE) ? W_ROOK : B_ROOK;
			board_state.halfmove_count = 0;
		}

		board[m.to.rank][m.to.file] = moving_piece;
	}

	// pawn moves
	else if (is_pawn(moving_piece))
	{
		board_state.halfmove_count = 0;

		// en passant if attacking and there is no piece
		if (m.from.file != m.to.file && !is_sq_occ(m.to))
		{
			board[m.from.rank][m.to.file] = EMPTY;
			board[m.to.rank][m.to.file] = moving_piece;
		}
		// double pawn push
		else if (m.to.rank == m.from.rank + 2 * turn)
		{
			board_state.ep_square = m.to;
			board[m.to.rank][m.to.file] = moving_piece;
		}
		// promotion
		else if (m.to.rank == 7 || m.to.rank == 0)
			board[m.to.rank][m.to.file] = Piece(m.promotion_or_enpassant);
		else
			board[m.to.rank][m.to.file] = moving_piece;
	}
	// rook moves revokes the castling rights
	else if (is_rook(moving_piece))
	{
		// update castling rights
		if (board_state.rights)
		{
			if (turn == WHITE)
			{
				if (m.from.file == 0)		board_state.set_wking_castle_queen(false);
				else if (m.from.file == 7)	board_state.set_wking_castle_king(false);
			}
			else
			{
				if (m.from.file == 0)		board_state.set_bking_castle_queen(false);
				else if (m.from.file == 7)	board_state.set_bking_castle_king(false);
			}
		}
		board[m.to.rank][m.to.file] = moving_piece;
	}
	else
	{
		board[m.to.rank][m.to.file] = moving_piece;
	}

	// change turn
	turn = static_cast<COLOR>(-turn);
	board_state.halfmove_count++;
	board[m.from.rank][m.from.file] = EMPTY;
}

void board_t::unmake_move(Move &m)
{
	// the turn variable is updated
	turn = static_cast<COLOR>(-turn);
	Piece moving_piece = piece_at(m.to);

	// king moves
	if (is_king(moving_piece))
	{
		// update position
		if (turn == WHITE) white_king = m.from;
		else black_king = m.from;

		// if king is jumping more than one square than castling
		// queen side castling
		if (m.from.file == 4 && m.to.file == 2)
		{
			board[m.to.rank][3] = EMPTY;
			board[m.to.rank][0] = (turn == WHITE) ? W_ROOK : B_ROOK;
		}
		if (m.from.file == 4 && m.to.file == 6)
		{
			board[m.to.rank][5] = EMPTY;
			board[m.to.rank][7] = (turn == WHITE) ? W_ROOK : B_ROOK;
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
	else if ((m.to.rank == 7 || m.to.rank == 0) && m.promotion_or_enpassant)
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
}

std::deque<Move> board_t::get_psuedo_legal_move_pawn(square_t sq) 
{
	std::deque<Move> moves;
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
			board_state.ep_square.rank == sq.rank && 
			(board_state.ep_square.file == sq.file - 1 || board_state.ep_square.file == sq.file + 1))
			moves.push_back(Move(sq, {sq.rank + color, board_state.ep_square.file}, piece_at(board_state.ep_square), true));
	}

	return moves;
}

std::deque<Move> board_t::get_psuedo_legal_move_knight(square_t sq) const
{
	std::deque<Move> moves;
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

std::deque<Move> board_t::get_psuedo_legal_move_bishop(square_t sq) const
{
	std::deque<Move> moves;
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

std::deque<Move> board_t::get_psuedo_legal_move_rook(square_t sq) const
{
	std::deque<Move> moves;
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

std::deque<Move> board_t::get_psuedo_legal_move_queen(square_t sq)
{
	std::deque<Move> bishop_moves = std::move(get_psuedo_legal_move_bishop(sq));
	std::deque<Move> rook_moves = std::move(get_psuedo_legal_move_rook(sq));
	rook_moves.insert(rook_moves.end(), std::make_move_iterator(bishop_moves.begin()), std::make_move_iterator(bishop_moves.end()));
	return rook_moves;
}

std::deque<Move> board_t::get_psuedo_legal_move_king(square_t sq) const
{
	std::deque<Move> moves;
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

bool board_t::is_sq_attacked_by(square_t sq, COLOR color) const
{
	square_t to;

	// check for pawn
	to = {sq.rank - color, sq.file + 1};
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		return true;
	to.file -= 2;
	if (to.inbound() && is_sq_occ(to) && piece_color(piece_at(to)) == color && is_pawn(piece_at(to)))
		return true;

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
	square_t to;
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

	return squares;
}

std::deque<Move> board_t::get_psuedo_legal_moves()
{
	std::deque<Move> moves;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			// if square is empty or different color
			if (piece_at(rank, file) == EMPTY || piece_color(piece_at(rank, file)) != turn)
				continue;
			else if (is_pawn(piece_at(rank, file)))
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_pawn({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_knight(piece_at(rank, file)))
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_knight({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_bishop(piece_at(rank, file)))
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_bishop({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_rook(piece_at(rank, file)))
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_rook({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else if (is_queen(piece_at(rank, file)))
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_queen({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
			else
			{
				std::deque<Move> temp = std::move(get_psuedo_legal_move_king({ rank, file }));
				moves.insert(moves.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
			}
		}
	}

	return moves;
}

void board_t::init_startpos()
{
	board = STARTING_POSITION;
	turn = WHITE;
	board_state.rights = 15;
	board_state.ep_square = INVALID_SQ;
	white_king = {0, 4};
	black_king = {7, 4};
}

void board_t::init_fen(std::deque<std::string> &command)
{
	reset();
	int i = 1;
	if (command[i] == "startpos")
	{
		init_startpos();
		i++;
	}
	else
	{
		i++;
		fen_to_board(command[i++]);
		turn = (command[i++] == "w") ? WHITE : BLACK;
		board_state.set_castling_rights(command[i++]);

		square_t temp = uci_to_sq(command[i]);
		if (command[i].size() != 1)
			temp.rank = temp.rank - turn;
		board_state.ep_square = temp;
		board_state.halfmove_count = std::stoi(command[++i]);
		// 7 is full move count
		i = 8;
	}

	// skipping moves command
	i++;
	while (i < command.size())
	{
		auto m = uci_to_move(command[i]);
		// null move
		if (m.from == INVALID_SQ || m.to == INVALID_SQ)
			make_null_move();
		else
			make_move(m);
		i++;
	}
}

void board_t::fen_to_board(std::string &fen)
{
	int rank = 7, file = -1;
	for (auto &c : fen)
	{
		if (c == '/')
		{
			rank -= 1;
			file = -1;
		}
		else if (isdigit(c))
		{
			file += c - '0';
		}
		else if (c == W_PAWN_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_PAWN;
		}
		else if (c == B_PAWN_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_PAWN;
		}
		else if (c == W_KNIGHT_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_KNIGHT;
		}
		else if (c == B_KNIGHT_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_KNIGHT;
		}
		else if (c == W_BISHOP_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_BISHOP;
		}
		else if (c == B_BISHOP_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_BISHOP;
		}
		else if (c == W_ROOK_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_ROOK;
		}
		else if (c == B_ROOK_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_ROOK;
		}
		else if (c == W_QUEEN_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_QUEEN;
		}
		else if (c == B_QUEEN_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_QUEEN;
		}
		else if (c == W_KING_SYMBOL)
		{
			file += 1;
			board[rank][file] = W_KING;
		}
		else if (c == B_KING_SYMBOL)
		{
			file += 1;
			board[rank][file] = B_KING;
		}
	}
}

Move board_t::uci_to_move(const std::string &move) const
{
	Piece promotion = EMPTY;
	// promotion
	if (move.size() == 5)
	{
		if (turn == WHITE)
		{
			switch (move[5])
			{
			case 'q':
				promotion = W_QUEEN;
				break;
			case 'n':
				promotion = W_KNIGHT;
				break;
			case 'b':
				promotion = W_BISHOP;
				break;
			case 'r':
				promotion = W_ROOK;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (move[5])
			{
			case 'q':
				promotion = B_QUEEN;
				break;
			case 'n':
				promotion = B_KNIGHT;
				break;
			case 'b':
				promotion = B_BISHOP;
				break;
			case 'r':
				promotion = B_ROOK;
				break;
			default:
				break;
			}
		}
	}

	// null move
	if (move == "0000")
		return Move();

	square_t from{move[1] - '1', move[0] - 'a'};
	square_t to{move[3] - '1', move[2] - 'a'};

	// check for en passant
	if (is_pawn(piece_at(from)) && from.file != to.file && !is_sq_occ(to))
		return Move(from, to, piece_at(from.rank, to.file), true);

	return Move(from, to, piece_at(to), promotion);
}

std::ostream &operator<<(std::ostream &out, const board_t &_b)
{
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			switch (_b.board[i][j])
			{
			case EMPTY:
				out << EMPTY_SYMBOL;
				break;
			case W_PAWN:
				out << W_PAWN_SYMBOL;
				break;
			case W_KNIGHT:
				out << W_KNIGHT_SYMBOL;
				break;
			case W_BISHOP:
				out << W_BISHOP_SYMBOL;
				break;
			case W_ROOK:
				out << W_ROOK_SYMBOL;
				break;
			case W_QUEEN:
				out << W_QUEEN_SYMBOL;
				break;
			case W_KING:
				out << W_KING_SYMBOL;
				break;
			case B_PAWN:
				out << B_PAWN_SYMBOL;
				break;
			case B_KNIGHT:
				out << B_KNIGHT_SYMBOL;
				break;
			case B_BISHOP:
				out << B_BISHOP_SYMBOL;
				break;
			case B_ROOK:
				out << B_ROOK_SYMBOL;
				break;
			case B_QUEEN:
				out << B_QUEEN_SYMBOL;
				break;
			case B_KING:
				out << B_KING_SYMBOL;
				break;
			}
			out << " ";
		}
		out << std::endl;
	}
	return out;
}
