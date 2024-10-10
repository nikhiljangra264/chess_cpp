#include "evaluate.h"
#include "psqt.h"

namespace Eval {

enum Phase {
    OPENING,
    MIDDLEGAME,
    ENDGAME
};
constexpr int MAX_ENDGAME    = 3500;
constexpr int MAX_MIDDLEGAME = 5000;

int evaluate(const board_t& _b) {
    auto material = 0;
    for (u8 rank = 0; rank < TOTAL_RANK; ++rank)
        for (u8 file = 0; file < TOTAL_FILE; ++file)
            material += PieceValue[_b.piece_at(rank, file)];

    // calculate phase
    material -= PieceValue[W_KING] * 2;
    auto phase = (material >= MAX_ENDGAME) ? MIDDLEGAME : OPENING;

    material        = 0;
    auto psqt_score = 0;
    // material and psqt
    for (u8 rank = 0; rank < TOTAL_RANK; ++rank)
    {
        for (u8 file = 0; file < TOTAL_FILE; ++file)
        {
            // if square is not occupied or the piece is of different color
            if (!_b.is_sq_occ(rank, file))
                continue;
            auto piece = _b.piece_at(rank, file);
            material += PieceValue[piece] * piece_color(piece);
            psqt_score +=
              (phase == MIDDLEGAME) ? psqt_middle[piece][rank][file] : psqt_end[piece][rank][file];
        }
    }

    return psqt_score + material;
}
};
