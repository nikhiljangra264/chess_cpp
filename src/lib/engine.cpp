#include "engine.h"

int Engine::absearch(int alpha, int beta, u16 depth)
{
    // Base case: if depth is 0 return evaluation
    if (depth == 0) {
        return eval::evaluate(board);
    }

    int best_score;
    bool prune = false;

    // Determine maximizing or minimizing player
    bool maximizing_player = (board.side_to_move() == WHITE);
    best_score = (maximizing_player ? -1:1) * INFINITE;

    // Get legal moves
    std::deque<Move> moves = std::move(board.get_psuedo_legal_moves());

    // Iterate over legal moves
    for (auto& move : moves) {
        // Make move
        auto temp = board.get_castling_rights();
        board.make_move(move);

        // Check if move maker is in check, if not then proceed
        if ((!maximizing_player && !board.is_sq_attacked_by(board.get_king_sq(BLACK), WHITE)) ||
            (maximizing_player && !board.is_sq_attacked_by(board.get_king_sq(WHITE), BLACK)))
        {
            // Recursively search for the best move
            int score = absearch(alpha, beta, depth - 1);

            // Update best score and alpha/beta
            if (maximizing_player) {
                if (score > best_score) {
                    best_score = score;
                    if (depth == max_depth)
                        best_move = move;
                    alpha = std::max(alpha, score);
                    if (alpha >= beta)
                        prune = true;
                }
            }
            else {
                if (score < best_score) {
                    best_score = score;
                    if (depth == max_depth)
                        best_move = move;
                    beta = std::min(beta, score);
                    if (alpha >= beta)
                        prune = true;
                }
            }
        }

        // Unmake move
        board.unmake_move(move);
        board.set_castling_rights(temp);

        // Prune if necessary
        if (prune)
            break;
    }

    return best_score;
}


void Engine::search()
{
    absearch(-1 * INFINITE, INFINITE, max_depth);

    std::cout << "bestmove " << to_uci(best_move) << "\n";
}

std::string Engine::to_uci(Move& m)
{
    std::string uci_format;

    uci_format.push_back('a' + m.from.file);
    uci_format.push_back('1' + m.from.rank);
    uci_format.push_back('a' + m.to.file);
    uci_format.push_back('1' + m.to.rank);

    if ((m.to.rank == 7 || m.to.rank == 0) && m.promotion_or_enpassant != EMPTY)
    {
        if (is_knight(Piece(m.promotion_or_enpassant)))
            uci_format.push_back(B_KNIGHT_SYMBOL);
        else if (is_queen(Piece(m.promotion_or_enpassant)))
            uci_format.push_back(B_QUEEN_SYMBOL);
    }
    return uci_format;
}
