#include "uci.h"

void UCI::stop() {
    // stop engine
    engine.stop();

    // join the thread
    if (thread && thread->joinable()) {
        thread->join();
    }
}

void UCI::eval() {
    std::string e = std::to_string(eval::evaluate(board));
    sync_cout << e << sync_endl;
}

void UCI::loop(int argc, const char* argv[])
{
    std::string token, cmd;

    for (int i = 1; i < argc; ++i)
        cmd += std::string(argv[i]) + " ";

    do
    {
        std::istringstream is(cmd);

        token.clear();  // Avoid a stale if getline() returns nothing or a blank line
        is >> std::skipws >> token;

        if (token == "quit")
        {
            stop();
            break;
        }
        else if (token == "stop")
            stop();

        else if (token == "uci")
            sync_cout << "id name chess_cpp" << "\n"
            << "id author Nikhil" << "\n"
            << "uciok" << sync_endl;

        else if (token == "go")
            go(is);

        else if (token == "position")
            position(is);

        else if (token == "ucinewgame")
            engine.reset();

        else if (token == "isready")
            sync_cout << "readyok" << sync_endl;

        // Add custom non-UCI commands, mainly for debugging purposes.
        // These commands must not be used during a search!
        else if (token == "eval")
            eval();

        else if (token == "print")
            sync_cout << board << sync_endl;

        else if (token == "--help" || token == "help" || token == "--license" || token == "license")
            sync_cout
            << "\nchess_cpp uses UCI protocol to communicate with a GUI, an API, etc."
            << "\nTry position, go, uci, readyok command"
            << "\nType quit to exit the engine."
            << "\nFor any further information, visit https://github.com/nikhiljangra264/chess_cpp#readme"
            << sync_endl;

        else if (!token.empty())
            sync_cout << "Unknown command: '" << cmd << "'. Type help for more information."
            << sync_endl;

        if (!std::getline(std::cin, cmd))
        {
            token = "quit";
            stop();
        }

    } while (token != "quit");
}

void UCI::position(std::istringstream& is)
{
    std::string token, fen;

    is >> token;

    if (token == "startpos")
    {
        fen = StartFEN;
        is >> token;  // Consume the "moves" token, if any
    }
    else if (token == "fen")
    {
        while (is >> token && token != "moves")
            fen += token + " ";
    }
    else
        return;

    std::vector<std::string> moves;

    while (is >> token)
    {
        moves.push_back(token);
    }

    board.set_position(fen, moves);
}

void UCI::go(std::istringstream& is)
{
    engine.limits = parse_limits(is);

    if (thread && thread->joinable()) {
        thread->join();
    }

    // Create a new thread and execute it
    thread = std::make_unique<std::thread>([this]() { engine.search(); });
}

limits_t UCI::parse_limits(std::istringstream& is)
{
    limits_t limits;
    std::string token;

    limits.start_time = std::chrono::steady_clock::now();  // The search starts as early as possible

    while (is >> token)
    {
        if (token == "wtime")
        {
            if (!(is >> limits.wtime))
            {
                throw std::invalid_argument("Error: Invalid value for wtime\n");
            }
        }
        else if (token == "btime")
        {
            if (!(is >> limits.btime))
            {
                throw std::invalid_argument("Error: Invalid value for btime\n");
            }
        }
        else if (token == "movetime")
        {
            if (!(is >> limits.movetime))
            {
                throw std::invalid_argument("Error: Invalid value for movetime\n");
            }
        }
        else if (token == "depth")
        {
            if (!(is >> limits.depth))
            {
                throw std::invalid_argument("Error: Invalid value for depth\n");
            }
        }
        else if (token == "nodes")
        {
            if (!(is >> limits.nodes))
            {
                throw std::invalid_argument("Error: Invalid value for nodes\n");
            }
        }
        else
        {
            throw std::invalid_argument("Warning: Unknown token\n");
        }
    }
    limits.movetime -= OVERHEAD_TIME;

    return limits;
}