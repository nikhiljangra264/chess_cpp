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
    // test making and unmaking move
    board_t board_make, final_position, starting_position;

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
    Move move19{square_t(6, 6), square_t(7, 5), B_BISHOP, W_QUEEN}; // g7f8q


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

    std::cout << board_make;

    std::deque<std::string> fen = {"position","fen","k2r1Q1r/ppp1pp1p/2nq4/8/8/2N5/PPPP1P1P/R1BQKb1R","b","KQ--","-","0","4"};
    final_position.init_fen(fen);

    EXPECT_EQ(board_make, final_position);

    // Unmake moves
    board_make.unmake_move(move19);
    board_make.unmake_move(move18);
    board_make.unmake_move(move17);
    board_make.unmake_move(move16);
    board_make.unmake_move(move15);
    board_make.unmake_move(move14);
    board_make.unmake_move(move13);
    board_make.unmake_move(move12);
    board_make.unmake_move(move11);
    board_make.unmake_move(move10);
    board_make.unmake_move(move9);
    board_make.unmake_move(move8);
    board_make.unmake_move(move7);
    board_make.unmake_move(move6);
    board_make.unmake_move(move5);
    board_make.unmake_move(move4);
    board_make.unmake_move(move3);
    board_make.unmake_move(move2);
    board_make.unmake_move(move1);
    // setting castling rights manually
    // we have to handle castling rights manually in game
    board_make.set_castling_rights(castling_rights(15));

    // Compare the resulting board state with the starting board state
    EXPECT_EQ(board_make, starting_position);
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

    // initialize another board
    std::deque<std::string> fen_1 = {"position","fen","r1bqkb1r/ppp1pppp/2nP1n2/8/8/2N5/PPPP1PPP/R1BQKBNR"};

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
