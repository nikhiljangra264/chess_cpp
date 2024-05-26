#include "uci.h"
#include "misc.h"
#include "tt.h"
#include "evaluate.h"

void UCI::stop() {
    // stop engine
    engine.stop();

    // Join the thread
    if (thread && thread->joinable()) {
        thread->join();
        thread.reset();
    }
}

void UCI::eval() {
    std::string e = std::to_string(Eval::evaluate(board));
    sync_cout << e << sync_endl;
}

void UCI::loop(int argc, const char* argv[])
{
    std::string token, cmd;

    for (int i = 1; i < argc; ++i)
        cmd += std::string(argv[i]) + " ";

    do
    {
        LOG::log_info("command received: " + cmd);

        std::istringstream is(cmd);

        token.clear();
        is >> std::skipws >> token;

        if (token == "quit")
        {
            stop();
            break;
        }
        else if (token == "stop")
            stop();

        else if (token == "uci")
            sync_cout << "id name chesscpp_3" << "\n"
            << "id author Nikhil" << "\n"
            << "option name Hash type spin default 128 min 1 max 65536\n"
            << "option name LogFile type check default false\n"
            << "uciok" << sync_endl;

        else if (token == "go")
            go(is);

        else if (token == "position")
            position(is);

        else if (token == "setoption")
            setoptions(is);

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
        is >> token;
    }
    else
    {
        while (is >> token && token != "moves")
            fen += token + " ";
    }

    std::vector<std::string> moves;

    while (is >> token)
    {
        moves.push_back(token);
    }

    board.set_position(fen, moves);
}

void UCI::go(std::istringstream& is)
{
    if (thread)
        stop();
    limits_t limits = parse_limits(is);
    std::lock_guard<std::mutex> lock(engine_mutex);
    thread = std::make_unique<std::thread>([this, limits]() { engine.go(board, limits); });
}

void UCI::setoptions(std::istringstream& is)
{
    // if engine running donot update the options
    std::lock_guard<std::mutex> lock(engine_mutex);

    options_t options;
    std::string token;

    while (is >> token)
    {
        if (token == "name")
        {
            is >> token;
            if (token == "Hash")
            {
                is >> token; // value
                if (!(is >> token)) {
                    options.tt_size = TT::DEFAULT_TT_SIZE;
                    LOG::log_error("Invalid value for tt size");
                }
            }
            else if (token == "LogFile")
            {
                is >> token;
                is >> token;
                if (token == "true")
                {
                    LOG::LOGGING = true;
                    LOG::open_log_file();
                }
                else
                {
                    LOG::LOGGING = false;
                    LOG::shutdown();
                }
            }
        }
    }
    if(options.tt_size != TT::tt.size())
        engine.set_options(options.tt_size);
}

limits_t UCI::parse_limits(std::istringstream& is)
{
    limits_t limits;
    std::string token;

    // start time as early as possible
    limits.start_time = std::chrono::high_resolution_clock::now();

    while (is >> token)
    {
        if (token == "depth")
        {
            if (!(is >> limits.depth))
            {
                limits.depth = MAX_DEPTH;
                LOG::log_error("Invalid depth value");
            }
        }
        else if (token == "nodes")
        {
            if (!(is >> limits.nodes))
            {
                limits.nodes = MAX_NODES;
                LOG::log_error("Invalid value for nodes");
            }
        }
        else if (token == "movetime")
        {
            if (!(is >> limits.movetime))
            {
                limits.nodes = MAX_TIME;
                LOG::log_error("Invalid value for movetime");
            }
        }
    }
    // limits.movetime -= OVERHEAD_TIME;
    return limits;
}