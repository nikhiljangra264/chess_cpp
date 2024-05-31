#include "search.h"
#include "evaluate.h"
#include <algorithm>
#include <cstring>

score_t Search::absearch(score_t alpha, score_t beta, depth_t depth, depth_t ply)
{
	if (check_time()) {
		return 0;
	}
	if (ply >= MAX_PLY) {
		return Eval::evaluate(board) * board.side_to_move();
	}

	TT::pv_length[ply] = ply;
	bool root_node = (ply == 0);
	hash_t key = board.get_key();

	if (!root_node) {
		if (board.history.position_occured(key)) {
			return -5; // slight draw bias
		}
		if (board.board_state.halfmove_count > 99) {
			return 0;
		}

		// mate distance pruning
		alpha = std::max(alpha, mated_in(ply));
		beta = std::min(beta, mate_in(ply));
		if (alpha >= beta) {
			return alpha;
		}
	}

	// quiescense search
	if (depth <= 0) {
		return qsearch(alpha, beta, ply);
	}

	// transposition table probing
	auto& tte = TT::tt[key];
	bool tt_hit = tte.key == key;
	Move tt_move = tt_hit ? tte.best_move : NULL_MOVE;
	score_t tt_score = tt_hit ? TT::tt.scoreFromTT(tte.score, ply) : VALUE_NONE;

	if (!root_node && tte.depth >= depth && tt_hit) {
		if (tte.type == TT::LOWER_BOUND) {
			alpha = std::max(alpha, tt_score);
		}
		else if (tte.type == TT::UPPER_BOUND) {
			beta = std::min(beta, tt_score);
		}

		if (alpha >= beta) {
			return tt_score;
		}
	}

	// to do null move pruning

	score_t oldAlpha = alpha;
	score_t best_score = -VALUE_INFINITE;
	Move best_move = NULL_MOVE;
	bool made_moves = false;

	if (!TT::moves_history.has_moves(key)) {
		MOVES temp = std::move(board.get_psuedo_legal_moves());
		TT::moves_history.store_moves(key, std::make_shared<MOVES>(temp));
	}

	auto moves = TT::moves_history.get_moves(key);

	std::sort(moves->begin(), moves->end(),
		[this, tt_move](Move& move1, Move& move2)
		{ return Eval::score_move(board, move1, tt_move) >
		Eval::score_move(board, move2, tt_move); }
	);

	for (auto& move : *moves) {
		board.push(move);
		if (__stop || !is_move_legal(move)) {
			board.pop();
			continue;
		}
		made_moves = true;
		nodes++;

		score_t score = -absearch(-beta, -alpha, depth - 1, ply + 1);

		board.pop();

		if (score > best_score) {
			best_score = score;
			best_move = move;

			TT::pv_table[ply][ply] = move;

			for (int i = ply + 1; i < TT::pv_length[ply + 1]; i++) {
				TT::pv_table[ply][i] = TT::pv_table[ply + 1][i];
			}

			TT::pv_length[ply] = TT::pv_length[ply + 1];

			if (score > alpha) {
				alpha = score;

				if (score >= beta) {
					// update history
					if (move.capture == EMPTY) {
						auto hhBonus = 
							(depth * depth) - 
							TT::h_table[(board.side_to_move() == WHITE) ? 0 : 1][move.from.rank][move.from.file][move.to.rank][move.to.file] 
							* abs(depth * depth) / 16384;

						TT::h_table[(board.side_to_move() == WHITE) ? 0 : 1]
							[move.from.rank][move.from.file][move.to.rank][move.to.file] += hhBonus;
					}

					break;
				}
			}
		}
	}

	// No moves were played so its checkmate or stalemate
	// Instead checking if the position is a checkmate during evaluate we can do it here
	// and save computation time
	if (!made_moves) {
			return board.is_check() ? mated_in(ply) : 0;
	}

	auto bound = (best_score >= beta) ? TT::LOWER_BOUND : (alpha != oldAlpha) ? TT::EXACT : TT::UPPER_BOUND;
	if(!check_time())
		TT::tt.store(key, depth, best_score, bound, best_move, ply);

	return best_score;
}

score_t Search::qsearch(score_t alpha, score_t beta, depth_t ply)
{
	if (__stop || check_time())
		return 0;

	if (ply >= MAX_PLY)
		return Eval::evaluate(board) * board.side_to_move();

	// static evaluation
	score_t best_value = Eval::evaluate(board) * board.side_to_move();
	if (best_value >= beta)
		return best_value;
	if (best_value > alpha)
		alpha = best_value;

	auto key = board.get_key();
	if (!TT::moves_history.has_captures(key)) {
		// Generate only capture moves initially for quiescence search
		MOVES temp_moves = std::move(board.get_psuedo_legal_moves());
		temp_moves.erase(std::remove_if(temp_moves.begin(), temp_moves.end(), [](const Move& move) {
			return move.capture == EMPTY;
			}), temp_moves.end());

		// Sort the moves, the highest score should come first,
		// to reduce the size of the search tree
		std::sort(temp_moves.begin(), temp_moves.end(), [this](const Move& move1, const Move& move2) {
			return Eval::score_q_move(board, move1) > Eval::score_q_move(board, move2);
			});
		
		TT::moves_history.store_captures(key, std::make_shared<MOVES>(temp_moves));
	}

	auto moves = TT::moves_history.get_captures(key);
	bool prune = false;

	for (auto& move : *moves) {
		board.push(move);

		if (!is_move_legal(move))
		{
			board.pop();
			continue;
		}

		nodes++;
		// delta pruning
		if (Eval::PieceValue[move.capture] + 400 + best_value
			< alpha && move.promotion_or_enpassant == EMPTY)
			continue;
		
		score_t score = -qsearch(-beta, -alpha, ply + 1);

		if (score > best_value) {
			best_value = score;

			if (score > alpha) {
				alpha = score;

				if (score >= beta)
					prune = true;
			}
		}

		board.pop();
		if (prune) break;
	}

	return best_value;
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
	return !board.is_oppo_check();
}

void Search::reset()
{
	cleanup();
	board.reset();
	TT::tt.clear();
	for (int i = 0; i < MAX_PLY; i++)
	{
		TT::pv_length[i] = 0;
		TT::pv_table[i].fill(NULL_MOVE);
	}
	memset(TT::h_table, 0, sizeof(TT::h_table));
}

void Search::iterative_deepening() {
	int score = -VALUE_INFINITE;
	Move best_move = NULL_MOVE;

	for (int depth = 1; depth <= limits.depth; ++depth) {

		// alpha beta search with increasing depth
		score = absearch(-VALUE_INFINITE, VALUE_INFINITE, depth, 0);

		if (__stop || check_time(true))
			break;

		best_move = TT::pv_table[0][0];

		// printing stats
		sync_cout << stats(depth, score, time()) << sync_endl;
		std::cout.flush();
	}

	if (best_move == NULL_MOVE) {
		best_move = TT::pv_table[0][0];
	}

	// print bestmove, as per UCI Protocol
	sync_cout << "bestmove " << best_move.to_uci() << sync_endl;
	std::cout.flush();

	cleanup();
}

std::string Search::stats(depth_t depth, score_t score, time_type time)
{
	time_type time_in_seconds = std::max(static_cast<time_type>(1), time / 1000);
	std::string info =
		"info depth " + std::to_string(depth) +
		" score " + std::move(convert_score(score)) +
		" nodes " + std::to_string(nodes) +
		" nps " + std::to_string(nodes / time_in_seconds) +
		" time " + std::to_string(time) +
		" pv" + std::move(getPV());
	return info;
}

inline std::string Search::getPV()
{
	std::string pv = "";
	for (int i = 0; i < TT::pv_length[0]; ++i) {
		pv += " " + TT::pv_table[0][i].to_uci();
	}
	return pv;
}

inline std::string Search::convert_score(score_t score)
{
	if (score >= VALUE_MATE_IN_PLY) {
		return "mate " + std::to_string(((VALUE_MATE - score) / 2) + ((VALUE_MATE - score) & 1));
	}
	else if (score <= VALUE_MATED_IN_PLY) {
		return "mate " + std::to_string(-((VALUE_MATE + score) / 2) + ((VALUE_MATE + score) & 1));
	}
	else {
		return "cp " + std::to_string(score);
	}
}
