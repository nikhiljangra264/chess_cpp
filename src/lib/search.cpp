#include "search.h"

int Search::absearch(int alpha, int beta, int depth, int ply)
{
	// Increase nodes count
	nodes++;
	if (nodes > limits.nodes || is_time_up())
	{
		__stop = true;
		return -INFINITE;
	}

	hash_t key = board.get_key();

	// If this position is already encountered before
	if (cache_positions.find(key) != cache_positions.end())
	{
		return score_table[key];
	}

	// Draw due to fifty-move rule
	if (board.board_state.halfmove_count > 99)
	{
		return 0;
	}

	// Base case: if depth is 0, return evaluation
	if (depth <= 0)
	{
		return eval::evaluate(board) * board.side_to_move();
	}

	bool is_make_any_move = false;
	bool prune = false;
	int max_score = -INFINITE;

	std::shared_ptr<std::deque<Move>> moves;

	if (moves_table.find(key) != moves_table.end())
	{
		moves = moves_table[key];
	}
	else
	{
		// Get pseudo-legal moves
		std::deque<Move> moves_temp = board.get_psuedo_legal_moves();

		// Sort the moves
		std::sort(moves_temp.begin(), moves_temp.end(), [](Move& a, Move& b) {
			return eval::score_move(a) > eval::score_move(b);
			});

		moves = std::make_shared<std::deque<Move>>(std::move(moves_temp));
		moves_table[key] = moves;
		score_table[key] = max_score;
	}

	cache_positions.insert(key);

	// traverse the moves
	for (auto& move : *moves)
	{
		if (__stop) return max_score;

		// make the move
		board.push(move);

		// check for legal move
		if (is_move_legal(move))
		{
			is_make_any_move = true;
			int score = -absearch(-beta, -alpha, depth - 1, ply+1);

			if (score > max_score)
			{
				max_score = score;
				score_table[key] = score;
				if (!ply)
				{
					best_move = move;
				}
				if (score > alpha)
				{
					alpha = score;
					if (alpha >= beta)
						prune = true;
				}
			}
		}

		// unmake move
		board.pop();
		if (prune) break;
	}

	if (!is_make_any_move)
	{
		// check for checkmate and stalemate
		if (board.is_sq_attacked_by(board.get_king_sq(board.side_to_move()), static_cast<COLOR>(-1*board.side_to_move())))
			// in check
			max_score = -CHECKMATE_VALUE * depth;
		else
			// stalemate
			max_score = STALEMATE_VALUE;
	}
	return max_score;
}

void Search::iterative_deepening()
{
	for (int depth = 1; depth <= limits.depth; ++depth)
	{
		cache_positions.clear();
		nodes = 0;
		absearch(-INFINITE, INFINITE, depth, 0);
	}
}

void Search::search()
{
	iterative_deepening();

	std::cout << "bestmove " << to_uci(best_move) << "\n";
	std::cout.flush();
	reset();
}

std::string Search::to_uci(Move& m)
{
	std::string uci_format;

	// null move
	if (m.from == INVALID_SQ || m.to == INVALID_SQ)
		return "0000";

	uci_format.push_back('a' + m.from.file);
	uci_format.push_back('1' + m.from.rank);
	uci_format.push_back('a' + m.to.file);
	uci_format.push_back('1' + m.to.rank);

	if ((m.to.rank == TOTAL_FILE || m.to.rank == 0) && m.promotion_or_enpassant != EMPTY)
	{
		if (is_knight(Piece(m.promotion_or_enpassant)))
			uci_format.push_back(PieceSymbol[B_KNIGHT]);
		else if (is_queen(Piece(m.promotion_or_enpassant)))
			uci_format.push_back(PieceSymbol[B_QUEEN]);
	}
	return uci_format;
}

bool Search::is_move_legal(Move& m)
{
	// if king castling
	if (is_king(board.piece_at(m.to)))
	{
		// queen side castling
		if (m.to.file == m.from.file - 2)
		{
			// check ex dx cx square
			if (board.is_sq_attacked_by({ m.from.rank,4 }, board.side_to_move()) ||
				board.is_sq_attacked_by({ m.from.rank,3 }, board.side_to_move()) ||
				board.is_sq_attacked_by({ m.from.rank,2 }, board.side_to_move()))
				return false;
			return true;
		}
		// king side castling
		if (m.from.file == m.to.file + 2)
		{
			// check ex fx gx square
			if (board.is_sq_attacked_by({ m.from.rank,4 }, board.side_to_move()) ||
				board.is_sq_attacked_by({ m.from.rank,5 }, board.side_to_move()) ||
				board.is_sq_attacked_by({ m.from.rank,6 }, board.side_to_move()))
				return false;
			return true;
		}
	}
	// not than simply check if the king in check
	return !board.is_sq_attacked_by(board.get_king_sq(static_cast<COLOR>(-1* board.side_to_move())), board.side_to_move());
}
