#include "tt.h"

namespace TT {

    std::array<int, MAX_PLY> pv_length;
    std::array<std::array<Move, MAX_PLY>, MAX_PLY> pv_table;
    score_t h_table[2][8][8][8][8] = { { { { 0 } } } };
    TranspositionTable tt;
    store_moves_t moves_history;

	void TranspositionTable::store(hash_t key, depth_t depth, score_t score, NodeType type, Move best_move, depth_t ply)
	{
        auto index = key % tt_size;
        auto& entry = table[index];

        if (entry.key != key || entry.best_move != best_move) {
            entry.best_move = best_move;
        }

        if (entry.key != key || type == NodeType::EXACT || depth + 4 > entry.depth) {
            entry.depth = depth;
            entry.score = scoreToTT(score, ply);
            entry.key = key;
            entry.type = type;
        }
	}
    int TT::TranspositionTable::scoreToTT(score_t s, depth_t plies) const
    {
        if (s >= VALUE_TB_WIN_IN_MAX_PLY) {
            return s + plies;
        }
        else if (s <= VALUE_TB_LOSS_IN_MAX_PLY) {
            return s - plies;
        }
        else {
            return s;
        }
    }
    int TT::TranspositionTable::scoreFromTT(score_t s, depth_t plies) const
    {
        if (s >= VALUE_TB_WIN_IN_MAX_PLY) {
            return s - plies;
        }
        else if (s <= VALUE_TB_LOSS_IN_MAX_PLY) {
            return s + plies;
        }
        else {
            return s;
        }
    }
}