#include "hashing.h"

namespace Zobrist {
    std::array<std::array<std::array<hash_t, TOTAL_FILE>, TOTAL_RANK>, NUM_PIECES> piece_keys;
    std::array<std::array<hash_t, TOTAL_FILE>, 2> en_passant_keys;
    std::array<hash_t, NUM_CASTLING_RIGHTS> castling_rights_keys;
    hash_t side_to_move_key;

    void init_zobrist_hash() {
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
            en_passant_keys[0][file] = dist(rng);  // 3rd rank
            en_passant_keys[1][file] = dist(rng);  // 5th rank
        }
    }
}