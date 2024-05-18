#ifndef __HASHING_H__
#define __HASHING_H__

#include <array>
#include <random>
#include <limits>
#include "header.h"

constexpr int NUM_PIECES = 12;  // types of pieces
constexpr int NUM_CASTLING_RIGHTS = 16;

inline std::array<std::array<std::array<hash_t, TOTAL_FILE>, TOTAL_RANK>, NUM_PIECES> piece_keys{};
inline std::array<std::array<hash_t, TOTAL_FILE>, 2> en_passant_keys{};  // One for each square on 3rd and 5th rank
inline std::array<hash_t, NUM_CASTLING_RIGHTS> castling_rights_keys{};
inline hash_t side_to_move_key{};

inline void init_zobrist_hash() {
    // Initialize random number generator
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<hash_t> dist(0, std::numeric_limits<hash_t>::max());

    // Generate random keys for pieces on each square
    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (u8 rank = 0; rank < TOTAL_RANK; ++rank) {
            for (u8 file = 0; file < TOTAL_FILE; ++file) {
                piece_keys[piece][rank][file] = dist(rng);
            }
        }
    }

    // Generate random key for side to move
    side_to_move_key = dist(rng);

    // Generate random keys for castling rights
    for (int i = 0; i < NUM_CASTLING_RIGHTS; ++i) {
        castling_rights_keys[i] = dist(rng);
    }

    // Generate random keys for en passant squares (3rd and 5th ranks)
    for (int file = 0; file < TOTAL_FILE; ++file) {
        en_passant_keys[0][file] = dist(rng);
        en_passant_keys[1][file] = dist(rng);
    }
}

#endif // __HASHING_H__
