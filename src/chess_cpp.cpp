﻿#include "lib/uci.h"
#include "lib/hashing.h"
#include "lib/misc.h"

int main(int argc, char const* argv[])
{
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