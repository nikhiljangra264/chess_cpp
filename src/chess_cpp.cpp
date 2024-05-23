#include "lib/uci.h"
#include "lib/hashing.h"
#include "lib/misc.h"

int main(int argc, char const* argv[])
{
    if(argc > 1 && argv[1] == "LOG")
    {
        LOG::LOGGING = true;
        if (argc > 2)
            LOG::open_log_file(std::string(argv[2]));
        else
            LOG::open_log_file();
        argc = 1;
    }
    // uci instance
    UCI uci;

    try {
        LOG::log_info("Initializing engine...");
        Zobrist::init_zobrist_hash();
        LOG::log_info("starting engine.");
        uci.loop(argc, argv);
    }
    catch (const std::exception& e) {
        LOG::log_error(std::string(e.what()));
        LOG::shutdown();
        return EXIT_FAILURE;
    }

    LOG::shutdown();

    return EXIT_SUCCESS;
}