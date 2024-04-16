#pragma once

// ok

#include <stdint.h>
#include <limits>
#include <array>


using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;
using s8 = char;

using time_type = uint32_t;
using score_t = int32_t;

enum COLOR :s8 {
    WHITE = 1,
    BLACK = -1
};

/// <summary>
/// piece representation
/// white king = 6
/// </summary>
enum Piece :u8 {
    EMPTY = 0,
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING = 6,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING
};

enum PieceSymbol :s8 {
    EMPTY_SYMBOL =      ' ',
    W_PAWN_SYMBOL =     'P',
    W_KNIGHT_SYMBOL =   'N',
    W_BISHOP_SYMBOL =   'B',
    W_ROOK_SYMBOL =     'R',
    W_QUEEN_SYMBOL =    'Q',
    W_KING_SYMBOL =     'K',
    B_PAWN_SYMBOL =     'p',
    B_KNIGHT_SYMBOL =   'n',
    B_BISHOP_SYMBOL =   'b',
    B_ROOK_SYMBOL =     'r',
    B_QUEEN_SYMBOL =    'q',
    B_KING_SYMBOL =     'k'
};

inline bool is_pawn(Piece _p)   { return _p == B_PAWN || _p == W_PAWN; }
inline bool is_knight(Piece _p) { return _p == B_KNIGHT || _p == W_KNIGHT ; }
inline bool is_bishop(Piece _p) { return _p == B_BISHOP || _p == W_BISHOP ; }
inline bool is_rook(Piece _p)   { return _p == B_ROOK || _p == W_ROOK ; }
inline bool is_queen(Piece _p)  { return _p == B_QUEEN || _p == W_QUEEN; }
inline bool is_king(Piece _p)   { return _p == B_KING || _p == W_KING ; }

inline COLOR piece_color(Piece _p) { return (_p <= W_KING)?WHITE:BLACK; }

struct square_t
{
    u8 file : 4;
    u8 rank : 4;

    square_t() = default;
    constexpr square_t(int _rank, int _file) : file(_file), rank(_rank) {}

    bool operator==(const square_t other) const { return file == other.file && rank == other.rank; }
    bool operator!=(const square_t other) const { return file != other.file || rank != other.rank; }

    inline bool inbound() const { return file < 8 && rank < 8; }
};

constexpr std::array<std::array<Piece, 8>, 8> STARTING_POSITION{ {
    {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK},
    {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
    {B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK}
} };

constexpr std::array<std::array<Piece, 8>, 8> EMPTY_POSITION{ {
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}
} };

constexpr std::array<std::array<s8, 2>, 8> KNIGHT_DIRECTIONS{ {
    {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}
} };

constexpr std::array<std::array<s8, 2>, 4> ROOK_DIRECTIONS{ {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0}
} };

constexpr std::array<std::array<s8, 2>, 4> BISHOP_DIRECTIONS{ {
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
} };

constexpr std::array<std::array<s8, 2>, 8> ALL_DIRECTIONS{ {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
} };

static square_t INVALID_SQ{ 15, 15 };

constexpr u16 MAX_DEPTH = 10;
constexpr time_type MAX_TIME = -1;
constexpr u32 MAX_NODES = -1;

constexpr score_t CHECKMATE_VALUE = 32000;
constexpr score_t STALEMATE_VALUE = -500;
constexpr score_t INFINITE = std::numeric_limits<score_t>::max();