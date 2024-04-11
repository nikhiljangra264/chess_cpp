#include "engine.h"

int Engine::absearch(int alpha, int beta, u16 depth)
{
    // Base case: if depth is 0 return evaluation
    if (depth == 0) {
        return eval::evaluate(board);
    }

    bool maximizing_player = board.side_to_move() == WHITE;
    bool is_make_any_move = false;
    int max_score = (maximizing_player)?-INFINITE:INFINITE;
    
    // get psuedo legal moves
    std::deque<Move> moves = std::move(board.get_psuedo_legal_moves());
    // sort the moves
    std::sort(  moves.begin(), 
                moves.end(), 
                [this](Move &a, Move &b) { 
                    return eval::score_move(this->board, a) > eval::score_move(this->board, b); 
                    });
    
    // traverse the moves
    for(auto& move:moves)
    {
        // make the move
        auto temp_rights = board.get_castling_rights();
        board.make_move(move);

        // maximizing player
        if(maximizing_player)
        {
            // if mover's king still in check then illegal move
            if(!board.is_sq_attacked_by(board.get_king_sq(WHITE), BLACK))
            {
                is_make_any_move = true;
                int score = absearch(alpha, beta, depth-1);

                if(max_score <= score)
                {
                    max_score = score;
                    if(depth == max_depth) best_move = move;
                    alpha = std::max(alpha, score);
                    if (alpha >= beta)
                        break;
                }
            }
        }
        else
        {
            // if mover's king still in check then illegal move
            if(!board.is_sq_attacked_by(board.get_king_sq(BLACK), WHITE))
            {
                is_make_any_move = true;
                int score = absearch(alpha, beta, depth-1);

                if(max_score >= score)
                {
                    max_score = score;
                    if(depth == max_depth) best_move = move;
                    beta = std::min(beta, score);
                    if (alpha >= beta)
                        break;
                }
            }
        }

        // unmake move
        board.unmake_move(move);
        board.set_castling_rights(temp_rights);
    }

    if(!is_make_any_move)
    {
        // check for checkmate and stalemate
        if(board.is_sq_attacked_by(board.get_king_sq(board.side_to_move()), (maximizing_player)?BLACK:WHITE))
            // in check
            max_score = maximizing_player ? CHECKMATE_VALUE : -CHECKMATE_VALUE;
        else
            // stalemate
            max_score = STALEMATE_VALUE;
    }
    return max_score;
}


void Engine::search()
{
    absearch(-INFINITE, INFINITE, max_depth);

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
