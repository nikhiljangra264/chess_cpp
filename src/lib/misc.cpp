#include "misc.h"

// Used to serialize access to std::cout
// to avoid multiple threads writing at the same time.
std::ostream& operator<<(std::ostream& os, SyncCout sc) {

    static std::mutex m;

    if (sc == IO_LOCK)
        m.lock();

    if (sc == IO_UNLOCK)
        m.unlock();

    return os;
}

void board_history_t::push(const Move move, const board_state_t board_state, hash_t key)
{
    move_history.push_back(move);
    board_state_history.push_back(board_state);
    key_history.push_back(key);
    key_cache[key]++;
}

void board_history_t::pop()
{
    if (!key_history.empty())
    {
        hash_t key = key_history.back();
        move_history.pop_back();
        board_state_history.pop_back();
        if (key_cache[key] <= 1)
            key_cache.erase(key);
        else
            key_cache[key]--;
        key_history.pop_back();
    }
}
