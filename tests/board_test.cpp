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

    // advanced
    board_t board_1, board_2;
    // make advanced moves
    std::deque<std::string> fen_1 = {"position","startpos ","oves","e4","e5","Nf3","Nc6","Bb5","a6","Ba4","Nf6","O-O","Be7","Re1","b5","Bb3","d6","c3","O-O","h3","Na5","Bc2","c5","d4","Qc7","Nbd2","cxd4","cxd4","Nc6","Nb3","a5","Be3","Bd7"};
    std::deque<std::string> fen_2 = {"position","fen","r1bq1rk1/1ppnbppp/p1n1p3/3pP3/3P1P2/2N1BN2/PPP1B1PP/R2Q1RK1","w","-","-","0","17"};

    board_1.init_fen(fen_1);
    board_2.init_fen(fen_2);
    EXPECT_EQ(board_1, board_2);

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
