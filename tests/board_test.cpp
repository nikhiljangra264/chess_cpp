#include <gtest/gtest.h>
#include "../src/lib/board.h"

// Define a fixture for your tests
class BoardTest : public ::testing::Test {
protected:
    // Set up common variables or objects needed for your tests
    BoardTest() {
        // Initialize anything needed before each test
    }

    // Clean up after each test
    virtual ~BoardTest() {
        // Free resources, if any
    }

};


TEST_F(BoardTest, make_unmake_moves) {
    Zobrist::init_zobrist_hash();

    // test making and unmaking move
    board_t board_make, final_position, starting_position;
    starting_position.set_position(StartFEN);
    board_make.set_position(StartFEN);

    // Apply the specified moves to the board
    Move move1{square_t(1, 4), square_t(3, 4)};                     // e2e4
    Move move2{square_t(7, 6), square_t(5, 5)};                     // g8f6
    Move move3{square_t(0, 1), square_t(2, 2)};                     // b1c3
    Move move4{square_t(7, 1), square_t(5, 2)};                     // b8c6
    Move move5{square_t(3, 4), square_t(4, 4)};                     // e4e5
    Move move6{square_t(6, 3), square_t(4, 3)};                     // d7d5
    Move move7{square_t(4, 4), square_t(5,3), B_PAWN, true};        // e5d6 (en passant)
    Move move8{square_t(7, 3), square_t(5, 3), W_PAWN};             // d8d6
    Move move9{square_t(0, 6), square_t(2, 7)};                     // g1h3
    Move move10{square_t(7, 2), square_t(2, 7), W_KNIGHT};          // c8h3
    Move move11{square_t(1, 6), square_t(3, 6)};                    // g2g4
    Move move12{square_t(7, 4), square_t(7, 2)};                    // e8c8
    Move move13{square_t(3, 6), square_t(4, 6)};                    // g4g5
    Move move14{square_t(2, 7), square_t(0, 5), W_BISHOP};          // h3f1
    Move move15{square_t(4, 6), square_t(5, 5), B_KNIGHT};          // g5f6
    Move move16{square_t(7, 2), square_t(7, 1)};                    // c8b8
    Move move17{square_t(5, 5), square_t(6, 6), B_PAWN};            // f6g7
    Move move18{square_t(7, 1), square_t(7, 0)};                    // b8a8
    Move move19{square_t(6, 6), square_t(7, 5), B_BISHOP, W_QUEEN}; // g7f8q (promotion)


    // Make moves
    board_make.push(move1);
    board_make.push(move2);
    board_make.push(move3);
    board_make.push(move4);
    board_make.push(move5);
    board_make.push(move6);
    board_make.push(move7);
    board_make.push(move8);
    board_make.push(move9);
    board_make.push(move10);
    board_make.push(move11);
    board_make.push(move12);
    board_make.push(move13);
    board_make.push(move14);
    board_make.push(move15);
    board_make.push(move16);
    board_make.push(move17);
    board_make.push(move18);
    board_make.push(move19);

    std::string fen = "k2r1Q1r/ppp1pp1p/2nq4/8/8/2N5/PPPP1P1P/R1BQKb1R b KQ-- - 1 4";
    final_position.set_position(fen);

    EXPECT_EQ(board_make, final_position);

    // Unmake moves
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();
    board_make.pop();

    // Compare the resulting board state with the starting board state
    EXPECT_EQ(board_make, starting_position);
}

TEST_F(BoardTest, double_pawn_push) {
    // test setting board with fen
    Zobrist::init_zobrist_hash();

    // setup starting position on the board
    board_t board1, board2,board3;
    std::string starting_fen_1 = StartFEN;
    std::vector<std::string> moves = { "e2e4" };
    std::string starting_fen_2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

    board1.set_position(starting_fen_1, moves);
    board2.set_position(starting_fen_2);
    EXPECT_EQ(board1, board2);

    board3.set_position(starting_fen_1);
    board1.pop();
    EXPECT_EQ(board1, board3);
}
