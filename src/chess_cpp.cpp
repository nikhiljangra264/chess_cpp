#include "lib/uci.h"

int main(int argc, char const* argv[])
{
    // uci instance
    UCI uci;

    init_zobrist_hash();
    
    uci.loop(argc, argv);

    return 0;
}
