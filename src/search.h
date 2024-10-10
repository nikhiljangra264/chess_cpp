#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <unordered_set>
#include <set>

#include "board.h"
#include "tt.h"
#include "misc.h"
#include "move.h"

class Search {
   public:
    Search() = default;

    // control
    void go(board_t& _board, limits_t _limits) {
        __stop = false;
        board  = _board;
        limits = _limits;
        iterative_deepening();
    }
    void stop() { __stop = true; }
    void set_options(int hash_size) { TT::tt.resize(hash_size); }

    // clean
    void cleanup() {
        rate   = CHECK_RATE;
        limits = limits_t();
        nodes  = 0;
        __stop = false;
    }
    void reset();

   private:
    // search
    void    iterative_deepening();
    score_t absearch(score_t alpha, score_t beta, depth_t depth, depth_t ply);
    score_t qsearch(score_t alpha, score_t beta, depth_t ply);

    // output
    std::string        stats(depth_t depth, score_t score, time_type time);
    inline std::string getPV();
    inline std::string convert_score(score_t score);

    bool is_move_legal(Move& m);

    // Time management
    time_type time() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::high_resolution_clock::now() - limits.start_time)
          .count();
    }
    bool check_time(bool iter = false) {
        if (__stop || nodes > limits.nodes)
            return true;
        if (rate > 0 && !iter)
        {
            rate--;
            return false;
        }
        else
        {
            rate = CHECK_RATE;
            if (time() >= limits.movetime)
                __stop = true;
        }
        return __stop;
    }

   private:
    board_t board;

    int      rate   = CHECK_RATE;
    u64      nodes  = 0;
    limits_t limits = limits_t();
    bool     __stop = false;
};


#endif  // SEARCH_H