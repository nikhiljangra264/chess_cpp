#ifndef __MISC_H__
#define __MISC_H__

#include <iosfwd>
#include <mutex>
#include <deque>
#include <unordered_map>

#include "header.h"
#include "move.h"

enum SyncCout {
    IO_LOCK,
    IO_UNLOCK
};
std::ostream& operator<<(std::ostream&, SyncCout);

#define sync_cout std::cout << IO_LOCK
#define sync_endl std::endl << IO_UNLOCK

class board_history_t
{
    std::deque<Move> move_history;
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
        key_cache(other.key_cache)
    {
    }

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
        key_cache(std::move(other.key_cache))
    {
    }

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

    void push(const Move move, const board_state_t board_state, hash_t key);
    void pop();
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
};



#endif // __MISC_H__