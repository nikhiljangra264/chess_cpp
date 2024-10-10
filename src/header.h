#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>
#include <array>
#include <chrono>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;
using s8  = char;

using time_type  = uint64_t;
using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using score_t    = int;
using hash_t     = u64;
using depth_t    = u16;

constexpr u8 TOTAL_FILE = 8;
constexpr u8 TOTAL_RANK = 8;

enum COLOR : s8 {
    WHITE = 1,
    BLACK = -1
};

/// <summary>
/// piece representation
/// white king = 5
/// </summary>
enum Piece : u8 {
    W_PAWN,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING = 5,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    EMPTY
};

constexpr char PieceSymbol[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '};

inline bool is_pawn(Piece _p) { return _p == B_PAWN || _p == W_PAWN; }
inline bool is_knight(Piece _p) { return _p == B_KNIGHT || _p == W_KNIGHT; }
inline bool is_bishop(Piece _p) { return _p == B_BISHOP || _p == W_BISHOP; }
inline bool is_rook(Piece _p) { return _p == B_ROOK || _p == W_ROOK; }
inline bool is_queen(Piece _p) { return _p == B_QUEEN || _p == W_QUEEN; }
inline bool is_king(Piece _p) { return _p == B_KING || _p == W_KING; }

inline COLOR piece_color(Piece _p) { return (_p <= W_KING) ? WHITE : BLACK; }

struct square_t {
    u8 file: 4;
    u8 rank: 4;

    square_t() = delete;
    constexpr square_t(int _rank, int _file) :
        file(_file),
        rank(_rank) {}

    bool operator==(const square_t other) const { return file == other.file && rank == other.rank; }
    bool operator!=(const square_t other) const { return file != other.file || rank != other.rank; }

    inline bool inbound() const { return file < TOTAL_FILE && rank < TOTAL_RANK; }
};

constexpr std::array<std::array<s8, 2>, 8> KNIGHT_DIRECTIONS{
  {{2, 1}, {2, -1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}}};

constexpr std::array<std::array<s8, 2>, 4> ROOK_DIRECTIONS{{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};

constexpr std::array<std::array<s8, 2>, 4> BISHOP_DIRECTIONS{{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};

constexpr std::array<std::array<s8, 2>, 8> ALL_DIRECTIONS{
  {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};

static constexpr square_t INVALID_SQ{15, 15};

constexpr int       CHECK_RATE = 256;
constexpr depth_t   MAX_DEPTH  = 10;
constexpr u64       MAX_NODES  = -1;
constexpr depth_t   MAX_PLY    = 60;
constexpr time_type MAX_TIME   = -1;

constexpr score_t VALUE_NONE         = 32002;
constexpr score_t VALUE_MATE         = 32000;
constexpr score_t VALUE_INFINITE     = 32001;
constexpr score_t VALUE_MATE_IN_PLY  = VALUE_MATE - MAX_PLY;
constexpr score_t VALUE_MATED_IN_PLY = -VALUE_MATE_IN_PLY;

constexpr score_t VALUE_TB_WIN             = VALUE_MATE_IN_PLY;
constexpr score_t VALUE_TB_LOSS            = -VALUE_TB_WIN;
constexpr score_t VALUE_TB_WIN_IN_MAX_PLY  = VALUE_TB_WIN - MAX_PLY;
constexpr score_t VALUE_TB_LOSS_IN_MAX_PLY = -VALUE_TB_WIN_IN_MAX_PLY;

constexpr int MAX_HISTORY_SIZE = 1 << 17;

constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Macros for mate calculations
#define mate_in(ply) (VALUE_MATE - (ply))
#define mated_in(ply) ((ply) -VALUE_MATE)


#endif  // HEADER_H