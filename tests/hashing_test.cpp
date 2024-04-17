#include <gtest/gtest.h>
#include "../src/lib/board.h"

// Define a fixture for your tests
class HashTest : public ::testing::Test {
protected:
    // Set up common variables or objects needed for your tests
    HashTest() {
        // Initialize anything needed before each test
    }

    // Clean up after each test
    virtual ~HashTest() {
        // Free resources, if any
    }

};


TEST_F(HashTest, test_hash_generator) {
    // test making and unmaking move
    board_t board_make, final_position, starting_position;

    // Apply the specified moves to the board
    Move move1{ square_t(1, 4), square_t(3, 4) };                     // e2e4
    Move move2{ square_t(7, 6), square_t(5, 5) };                     // g8f6
    Move move3{ square_t(0, 1), square_t(2, 2) };                     // b1c3
    Move move4{ square_t(7, 1), square_t(5, 2) };                     // b8c6
    Move move5{ square_t(3, 4), square_t(4, 4) };                     // e4e5
    Move move6{ square_t(6, 3), square_t(4, 3) };                     // d7d5
    Move move7{ square_t(4, 4), square_t(5,3), B_PAWN, true };        // e5d6 (en passant)
    Move move8{ square_t(7, 3), square_t(5, 3), W_PAWN };             // d8d6
    Move move9{ square_t(0, 6), square_t(2, 7) };                     // g1h3
    Move move10{ square_t(7, 2), square_t(2, 7), W_KNIGHT };          // c8h3
    Move move11{ square_t(1, 6), square_t(3, 6) };                    // g2g4
    Move move12{ square_t(7, 4), square_t(7, 2) };                    // e8c8
    Move move13{ square_t(3, 6), square_t(4, 6) };                    // g4g5
    Move move14{ square_t(2, 7), square_t(0, 5), W_BISHOP };          // h3f1
    Move move15{ square_t(4, 6), square_t(5, 5), B_KNIGHT };          // g5f6
    Move move16{ square_t(7, 2), square_t(7, 1) };                    // c8b8
    Move move17{ square_t(5, 5), square_t(6, 6), B_PAWN };            // f6g7
    Move move18{ square_t(7, 1), square_t(7, 0) };                    // b8a8
    Move move19{ square_t(6, 6), square_t(7, 5), B_BISHOP, W_QUEEN }; // g7f8q

    // Make moves
    board_make.make_move(move1);
    board_make.make_move(move2);
    board_make.make_move(move3);
    board_make.make_move(move4);
    board_make.make_move(move5);
    board_make.make_move(move6);
    board_make.make_move(move7);
    board_make.make_move(move8);
    board_make.make_move(move9);
    board_make.make_move(move10);
    board_make.make_move(move11);
    board_make.make_move(move12);
    board_make.make_move(move13);
    board_make.make_move(move14);
    board_make.make_move(move15);
    board_make.make_move(move16);
    board_make.make_move(move17);
    board_make.make_move(move18);
    board_make.make_move(move19);

    std::deque<std::string> fen = { "position","fen","k2r1Q1r/ppp1pp1p/2nq4/8/8/2N5/PPPP1P1P/R1BQKb1R","b","KQ--","-","1","4" };
    final_position.init_fen(fen);

    EXPECT_EQ(board_make.get_key(), final_position.get_key());

    // Unmake moves
    board_make.unmake_move(move19, starting_position.board_state);
    board_make.unmake_move(move18, starting_position.board_state);
    board_make.unmake_move(move17, starting_position.board_state);
    board_make.unmake_move(move16, starting_position.board_state);
    board_make.unmake_move(move15, starting_position.board_state);
    board_make.unmake_move(move14, starting_position.board_state);
    board_make.unmake_move(move13, starting_position.board_state);
    board_make.unmake_move(move12, starting_position.board_state);
    board_make.unmake_move(move11, starting_position.board_state);
    board_make.unmake_move(move10, starting_position.board_state);
    board_make.unmake_move(move9, starting_position.board_state);
    board_make.unmake_move(move8, starting_position.board_state);
    board_make.unmake_move(move7, starting_position.board_state);
    board_make.unmake_move(move6, starting_position.board_state);
    board_make.unmake_move(move5, starting_position.board_state);
    board_make.unmake_move(move4, starting_position.board_state);
    board_make.unmake_move(move3, starting_position.board_state);
    board_make.unmake_move(move2, starting_position.board_state);
    board_make.unmake_move(move1, starting_position.board_state);

    // Compare the resulting board state with the starting board state
    EXPECT_EQ(board_make.get_key(), starting_position.get_key());
}
