#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <string>
#include <memory>
#include <algorithm>
#include <chrono>
#include <utility>
#include <set>
#include <unordered_map>

#include "limits.h"
#include "board.h"
#include "evaluate.h"

class Search
{
private:
    board_t& board;
    Move best_move;
    uint64_t nodes = 0;

    std::unordered_map<hash_t, std::deque<Move>> moves_table;
    std::unordered_map<hash_t, score_t> score_table;
    std::set<hash_t> cache_positions;

    bool __stop = false;
public:
    limits_t limits;

    // Constructor
    Search(board_t& _board) : board(_board) {}

    // Main search algorithm
    void iterative_deepening();
    int absearch(int alpha, int beta, int depth, int ply);
    void search();

    // Utility functions
    std::string to_uci(Move& m);
    bool is_move_legal(Move& m);

    // Time management
    time_type time() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - limits.start_time).count();
    }
    bool is_time_up() const
    {
        return time() >= limits.movetime;
    }

    // Control functions
    void stop()
    {
        __stop = true;
    }
    void reset()
    {
        best_move = Move();
        limits = limits_t();
        moves_table.clear();
        score_table.clear();
        cache_positions.clear();
    }
};

#endif // __SEARCH_H__