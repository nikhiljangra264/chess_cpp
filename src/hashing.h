#ifndef HASHING_H
#define HASHING_H

#include <array>
#include <random>
#include <limits>
#include "header.h"

namespace Zobrist {

constexpr int NUM_PIECES          = 12;  // Number of different piece types
constexpr int NUM_CASTLING_RIGHTS = 16;  // Maximum number of castling rights

extern std::array<std::array<std::array<hash_t, TOTAL_FILE>, TOTAL_RANK>, NUM_PIECES> piece_keys;
extern std::array<std::array<hash_t, TOTAL_FILE>, 2> en_passant_keys;
extern std::array<hash_t, NUM_CASTLING_RIGHTS>       castling_rights_keys;
extern hash_t                                        side_to_move_key;

// Function to initialize Zobrist hash keys
void init_zobrist_hash();

}  // namespace Zobrist

#endif  // HASHING_H
