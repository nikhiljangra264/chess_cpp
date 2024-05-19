#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdint.h>
#include <limits>
#include <array>
#include <chrono>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;
using s8 = char;

using time_type = uint64_t;
using time_point = std::chrono::time_point<std::chrono::steady_clock>;
using score_t = int;
using hash_t = u64;

constexpr u8 TOTAL_FILE = 8;
constexpr u8 TOTAL_RANK = 8;

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

constexpr char PieceSymbol[] = { ' ','P','N','B','R','Q','K','p','n','b','r','q','k' };

inline bool is_pawn(Piece _p)   { return _p == B_PAWN || _p == W_PAWN; }
inline bool is_knight(Piece _p) { return _p == B_KNIGHT || _p == W_KNIGHT ; }
inline bool is_bishop(Piece _p) { return _p == B_BISHOP || _p == W_BISHOP ; }
inline bool is_rook(Piece _p)   { return _p == B_ROOK || _p == W_ROOK ; }
inline bool is_queen(Piece _p)  { return _p == B_QUEEN || _p == W_QUEEN; }
inline bool is_king(Piece _p)   { return _p == B_KING || _p == W_KING ; }

inline COLOR piece_color(Piece _p) { return (_p <= W_KING) ? WHITE : BLACK; }

struct square_t
{
    u8 file;
    u8 rank;

    square_t() = delete;
    constexpr square_t(int _rank, int _file) : file(_file), rank(_rank) {}

    bool operator==(const square_t other) const { return file == other.file && rank == other.rank; }
    bool operator!=(const square_t other) const { return file != other.file || rank != other.rank; }

    inline bool inbound() const { return file < TOTAL_FILE && rank < TOTAL_RANK; }
};

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

static constexpr square_t INVALID_SQ{ 15, 15 };

constexpr int MAX_DEPTH = 10;
constexpr time_type MAX_TIME = -1;
constexpr u64 MAX_NODES = -1;
constexpr time_type OVERHEAD_TIME = 30;

constexpr score_t CHECKMATE_VALUE = 32000;
constexpr score_t STALEMATE_VALUE = -500;
constexpr score_t INFINITE = std::numeric_limits<score_t>::max();
constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


#endif // __HEADER_H__