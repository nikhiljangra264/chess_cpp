#include "uci.h"
#include "hashing.h"
#include "misc.h"

int main(int argc, char const* argv[]) {
    // uci instance
    UCI uci;

    try
    {
        Zobrist::init_zobrist_hash();
        uci.loop(argc, argv);
    } catch (const std::exception& e)
    {
        LOG::log_error(std::string(e.what()));
        LOG::shutdown();
        return EXIT_FAILURE;
    }

    LOG::shutdown();

    return EXIT_SUCCESS;
}