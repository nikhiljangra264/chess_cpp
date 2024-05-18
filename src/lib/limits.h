#ifndef __LIMITS_H__
#define __LIMITS_H__

#include "header.h"
#include <chrono>

struct limits_t {
    time_type wtime;
    time_type btime;
    time_type movetime;
    time_point start_time;
    u64 nodes;
    int depth;

    limits_t()
        : wtime(MAX_TIME),
        btime(MAX_TIME),
        movetime(MAX_TIME),
        start_time(std::chrono::steady_clock::now()),
        nodes(MAX_NODES),
        depth(MAX_DEPTH) {}
};

#endif // __LIMITS_H__
