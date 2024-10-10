#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <vector>
#include <cstdint>
#include <memory>
#include "header.h"
#include "move.h"

namespace TT {

// TTEntry size 24 bytes
// TTEntry in 1 MB or 1048576 bytes are 43690 (approx.)
constexpr int    MUL_FACTOR      = 43690;
constexpr size_t DEFAULT_TT_SIZE = 128;

enum NodeType : u16 {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND,
    NO_BOUND
};

struct TTEntry {
    hash_t   key       = 0;
    score_t  score     = 0;
    Move     best_move = NULL_MOVE;
    NodeType type      = NO_BOUND;
    depth_t  depth     = 0;
};

class TranspositionTable {
   public:
    TranspositionTable() :
        tt_size(DEFAULT_TT_SIZE * MUL_FACTOR),
        table(DEFAULT_TT_SIZE * MUL_FACTOR) {}

    // Store a new entry in the transposition table
    void
    store(hash_t key, depth_t depth, score_t score, NodeType type, Move best_move, depth_t ply);
    // Access entry by key
    TTEntry& operator[](hash_t key) { return table[key % tt_size]; }

    // Resize the transposition table
    void resize(int size) {
        tt_size = MUL_FACTOR * size;
        table.resize(tt_size);
    }

    // Clear the transposition table
    void clear() { table = std::vector<TTEntry>(tt_size); }

    // Convert score to table storage format by adjusting distance
    int scoreToTT(score_t s, depth_t plies) const;
    // Convert score from table storage format by adjusting distance
    int scoreFromTT(score_t s, depth_t plies) const;

    // Get the size of the transposition table
    size_t size() const { return tt_size; }

   private:
    size_t               tt_size;
    std::vector<TTEntry> table;
};

// store moves
class store_moves_t {
   public:
    bool has_moves(hash_t key) const { return key == moves_history[key % MAX_HISTORY_SIZE].first; }
    std::shared_ptr<MOVES> get_moves(hash_t key) {
        if (has_moves(key))
            return moves_history[key % MAX_HISTORY_SIZE].second;
        return {};
    }

    bool has_captures(hash_t key) const {
        return key == captures_history[key % MAX_HISTORY_SIZE].first;
    }
    std::shared_ptr<MOVES> get_captures(hash_t key) {
        if (has_captures(key))
            return captures_history[key % MAX_HISTORY_SIZE].second;
        return {};
    }
    void store_moves(hash_t key, std::shared_ptr<MOVES> moves) {
        moves_history[key % MAX_HISTORY_SIZE] = std::make_pair(key, moves);
    }
    void store_captures(hash_t key, std::shared_ptr<MOVES> captures) {
        captures_history[key % MAX_HISTORY_SIZE] = std::make_pair(key, captures);
    }

   private:
    std::pair<hash_t, std::shared_ptr<MOVES>> moves_history[MAX_HISTORY_SIZE];
    std::pair<hash_t, std::shared_ptr<MOVES>> captures_history[MAX_HISTORY_SIZE];
};

extern std::array<int, MAX_PLY>                       pv_length;
extern std::array<std::array<Move, MAX_PLY>, MAX_PLY> pv_table;
extern TranspositionTable                             tt;
// History Table
// Indexed by[color][from][to]
extern score_t h_table[2][8][8][8][8];

// store moves generated so that we donot have to calculate again and again
extern store_moves_t moves_history;

}  // namespace TT

#endif  // TRANSPOSITION_TABLE_H
