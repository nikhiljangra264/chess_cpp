#include "misc.h"
#include <iomanip>

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

namespace LOG {

bool          LOGGING = false;
std::ofstream log_file;
std::string   FILE_NAME = "chess_cpp.log";

void open_log_file(const std::string& file_name) {
    shutdown();
    log_file.open(file_name, std::ios::out | std::ios::app);
    // Check if the log file was opened successfully
    if (!log_file.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + file_name);
    }
}

void log_error(const std::string& message) {
    if (!LOGGING)
        return;
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    log_file << "ERROR: [" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] "
             << message << std::endl;
}

void log_info(const std::string& message) {
    if (!LOGGING)
        return;
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    log_file << "INFO: [" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "] "
             << message << std::endl;
}

void shutdown() {
    if (log_file.is_open())
    {
        log_file.close();
    }
}
}
