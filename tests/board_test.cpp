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


TEST_F(BoardTest, making_unmaking_move) {
    // test making and unmaking move
    board_t board1, starting_board;

    // Apply the specified moves to the board
    Move move1{square_t(1, 4), square_t(3, 4)};  // e2e4
    Move move2{square_t(7, 6), square_t(5, 5)};  // g8f6
    Move move3{square_t(0, 1), square_t(2, 2)};  // b1c3
    Move move4{square_t(7, 1), square_t(5, 2)};  // b8c6
    Move move5{square_t(3, 4), square_t(4, 4)};  // e4e5
    Move move6{square_t(6, 3), square_t(4, 3)};  // d7d5
    Move move7{square_t(4, 4), square_t(5,3)};  // e5d6 (en passant)
    
    // Make moves
    board1.make_move(move1);
    board1.make_move(move2);
    board1.make_move(move3);
    board1.make_move(move4);
    board1.make_move(move5);
    board1.make_move(move6);
    board1.make_move(move7);
    //board1.make_move(move8);

    // Unmake moves
    //board1.unmake_move(move8);
    board1.unmake_move(move7);
    board1.unmake_move(move6);
    board1.unmake_move(move5);
    board1.unmake_move(move4);
    board1.unmake_move(move3);
    board1.unmake_move(move2);
    board1.unmake_move(move1);

    // Compare the resulting board state with the starting board state
    EXPECT_EQ(starting_board, board1);
}

TEST_F(BoardTest, setting_board_with_fen) {
    // test setting board with fen

    // setup starting position on the board
    board_t starting_board, board;
    std::deque<std::string> starting_fen_1 = {"position","startpos"};
    std::deque<std::string> starting_fen_2 = {"position","fen","rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR","w","KQkq","-","0","1"};

    board.init_fen(starting_fen_1);
    EXPECT_EQ(starting_board, board);

    board.init_fen(starting_fen_2);
    EXPECT_EQ(starting_board, board);
/*
    // advanced
    board_t board_1, board_2;
    // make advanced moves
    std::deque<std::string> fen_1 = {"position","startpos ","oves","e4","e5","Nf3","Nc6","Bb5","a6","Ba4","Nf6","O-O","Be7","Re1","b5","Bb3","d6","c3","O-O","h3","Na5","Bc2","c5","d4","Qc7","Nbd2","cxd4","cxd4","Nc6","Nb3","a5","Be3","Bd7"};
    std::deque<std::string> fen_2 = {"position","fen","r1bq1rk1/1ppnbppp/p1n1p3/3pP3/3P1P2/2N1BN2/PPP1B1PP/R2Q1RK1","w","-","-","0","17"};

    board_1.init_fen(fen_1);
    board_2.init_fen(fen_2);
    EXPECT_EQ(board_1, board_2);
*/
    // Test making moves
    // Add your assertions here to verify the correctness of move making
    // For example:
    // Move move = ...; // Create a move object
    // board.make_move(move);
    // EXPECT_EQ(board.piece_at(destination_square), expected_piece);

    // Test unmaking moves
    // Add your assertions here to verify the correctness of move unmaking
    // For example:
    // board.unmake_move(move);
    // EXPECT_EQ(board.piece_at(start_square), moved_piece);
}
