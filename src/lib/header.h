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
/// king queen rook bishop night pawn color empty
/// </summary>
enum Piece :u8 {
    EMPTY =     0b00000000,
    W_PAWN =    0b00000110,
    W_KNIGHT =  0b00001010,
    W_BISHOP =  0b00010010,
    W_ROOK =    0b00100010,
    W_QUEEN =   0b01000010,
    W_KING =    0b10000010,
    B_PAWN =    0b00000100,
    B_KNIGHT =  0b00001000,
    B_BISHOP =  0b00010000,
    B_ROOK =    0b00100000,
    B_QUEEN =   0b01000000,
    B_KING =    0b10000000,
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

inline bool is_pawn(Piece _p) { return _p   & static_cast<u8>(4); }
inline bool is_knight(Piece _p) { return _p & static_cast<u8>(8); }
inline bool is_bishop(Piece _p) { return _p & static_cast<u8>(16); }
inline bool is_rook(Piece _p) { return _p   & static_cast<u8>(32); }
inline bool is_queen(Piece _p) { return _p  & static_cast<u8>(64); }
inline bool is_king(Piece _p) { return _p   & static_cast<u8>(128); }

inline COLOR piece_color(Piece _p) { return (_p & static_cast<u8>(2)) ? WHITE : BLACK; }

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