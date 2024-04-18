#pragma once

#include <random>

#include "header.h"

using hash_t = u64;

static hash_t piece_keys[12][8][8];
static hash_t side_to_move_key;
static hash_t en_passant_keys[2][8]; // One for each square on 4th rank and 5th rank file
static hash_t castling_rights_keys[16];

class zobrist_hasher {
public:
    static void init() {
        // Initialize random number generator
        std::mt19937_64 rng(std::random_device{}());
        std::uniform_int_distribution<hash_t> dist(0, std::numeric_limits<hash_t>::max());

        // Generate random keys for pieces on each square
        
        for (int piece = 0; piece < 12; ++piece) 
        {
            for (u8 rank = 0; rank < 8; rank++)
            {
                for (u8 file = 0; file < 8; file++)
                {
                    piece_keys[piece][rank][file] = dist(rng);
                }
            }
        }

        // Generate random keys for side to move
        side_to_move_key = dist(rng);

        // Generate random keys for castling rights
        for (int i = 0; i < 16; ++i) {
            castling_rights_keys[i] = dist(rng);
        }

        // Generate random keys for en passant squares
        // for file 4 and file 5
        for (int i = 0; i < 8; i++)
        {
            en_passant_keys[0][i] = dist(rng);
            en_passant_keys[1][i] = dist(rng);
        }
    }
};
