#include "engine.h"

int Engine::absearch(int alpha, int beta, u8 depth)
{
	// increase node traveled
	nodes++;
	if (nodes > max_nodes || is_time_up())
	{
		stop = true;
		return (board.side_to_move() == WHITE) ? -INFINITE : INFINITE;
	}

	// draw
	if (board.board_state.halfmove_count >= 50)
		return 0;
	
	// Base case: if depth is 0 return evaluation
	if (depth == 0) {
		return eval::evaluate(board);
	}

	bool maximizing_player = board.side_to_move() == WHITE;
	bool is_make_any_move = false;
	bool prune = false;
	int max_score = (maximizing_player) ? -INFINITE : INFINITE;

	// get psuedo legal moves
	std::deque<Move> moves = std::move(board.get_psuedo_legal_moves());
	// sort the moves
	std::sort(moves.begin(),
		moves.end(),
		[](Move& a, Move& b) {
			return eval::score_move(a) > eval::score_move(b);
		});

	// traverse the moves
	for (auto& move : moves)
	{
		if (stop) return max_score;
		// make the move
		auto temp_board_state = board.board_state;
		board.make_move(move);

		// maximizing player
		if (maximizing_player)
		{
			// if mover's king still in check then illegal move
			if (is_move_legal(move, WHITE, BLACK))
			{
				is_make_any_move = true;
				int score = absearch(alpha, beta, depth - 1);

				if (max_score < score)
				{
					max_score = score;
					if (depth == max_depth)
					{
						best_move = move;
						// print_stats();
					}
					alpha = std::max(alpha, score);
					if (alpha >= beta)
						prune = true;
				}
			}
		}
		else
		{
			// if mover's king still in check then illegal move
			if (is_move_legal(move, BLACK, WHITE))
			{
				is_make_any_move = true;
				int score = absearch(alpha, beta, depth - 1);

				if (max_score > score)
				{
					max_score = score;
					if (depth == max_depth)
					{
						best_move = move;
						// print_stats();
					}
					beta = std::min(beta, score);
					if (alpha >= beta)
						prune = true;
				}
			}
		}

		// unmake move
		board.board_state = temp_board_state;
		board.unmake_move(move);
		if (prune) break;
	}

	if (!is_make_any_move)
	{
		// check for checkmate and stalemate
		if (board.is_sq_attacked_by(board.get_king_sq(board.side_to_move()), (maximizing_player) ? BLACK : WHITE))
			// in check
			max_score = (maximizing_player ? -CHECKMATE_VALUE : +CHECKMATE_VALUE) * depth;
		else
			// stalemate
			max_score = maximizing_player ? -STALEMATE_VALUE : STALEMATE_VALUE;
	}
	return max_score;
}


void Engine::search()
{
	start_time = std::chrono::steady_clock::now();

	absearch(-INFINITE, INFINITE, max_depth);

	std::cout << "bestmove " << to_uci(best_move) << "\n";
	std::cout.flush();
}

std::string Engine::to_uci(Move& m)
{
	std::string uci_format;

	// null move
	if (m.from == INVALID_SQ || m.to == INVALID_SQ)
		return "0000";

	uci_format.push_back('a' + m.from.file);
	uci_format.push_back('1' + m.from.rank);
	uci_format.push_back('a' + m.to.file);
	uci_format.push_back('1' + m.to.rank);

	if ((m.to.rank == 7 || m.to.rank == 0) && m.promotion_or_enpassant != EMPTY)
	{
		if (is_knight(Piece(m.promotion_or_enpassant)))
			uci_format.push_back(B_KNIGHT_SYMBOL);
		else if (is_queen(Piece(m.promotion_or_enpassant)))
			uci_format.push_back(B_QUEEN_SYMBOL);
	}
	return uci_format;
}

bool Engine::is_move_legal(Move& m, COLOR move_maker, COLOR opposite)
{
	// if king castling
	if (is_king(board.piece_at(m.to)))
	{
		// queen side castling
		if (m.to.file == m.from.file - 2)
		{
			// check ex dx cx square
			if (board.is_sq_attacked_by({ m.from.rank,4 }, opposite) ||
				board.is_sq_attacked_by({ m.from.rank,3 }, opposite) ||
				board.is_sq_attacked_by({ m.from.rank,2 }, opposite))
				return false;
			return true;
		}
		// king side castling
		if (m.from.file == m.to.file + 2)
		{
			// check ex fx gx square
			if (board.is_sq_attacked_by({ m.from.rank,4 }, opposite) ||
				board.is_sq_attacked_by({ m.from.rank,5 }, opposite) ||
				board.is_sq_attacked_by({ m.from.rank,6 }, opposite))
				return false;
			return true;
		}
	}
	// not than simply check if the king in check
	return !board.is_sq_attacked_by(board.get_king_sq(move_maker), opposite);
}
