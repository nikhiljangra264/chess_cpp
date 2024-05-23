#ifndef MISC_H
#define MISC_H

#include <iosfwd>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>

#include "header.h"
#include "move.h"
#include "tt.h"

enum SyncCout {
    IO_LOCK,
    IO_UNLOCK
};
std::ostream& operator<<(std::ostream&, SyncCout);

#define sync_cout std::cout << IO_LOCK
#define sync_endl std::endl << IO_UNLOCK

namespace LOG {
    extern bool LOGGING;
    extern std::ofstream log_file;
    extern std::string FILE_NAME;

    void open_log_file(const std::string& file_name = FILE_NAME);
    void log_error(const std::string& message);
    void log_info(const std::string& message);
    void shutdown();
}

// to do
// implement other limits and options

struct limits_t {
    int depth;
    u64 nodes;
    time_type movetime;
    time_point start_time;

    limits_t() :
        depth(MAX_DEPTH), nodes(MAX_NODES), movetime(MAX_TIME) {}
};

struct options_t {
    int tt_size;

    options_t() :
        tt_size(TT::DEFAULT_TT_SIZE) {}
};



#endif // MISC_H