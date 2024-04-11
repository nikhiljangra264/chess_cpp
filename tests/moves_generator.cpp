#include <gtest/gtest.h>
#include <algorithm>
#include "../src/lib/board.h"

// Define a fixture for your tests
class MovesGeneratorTest : public ::testing::Test {
protected:
    // Set up common variables or objects needed for your tests
    MovesGeneratorTest() {
        // Initialize anything needed before each test
    }

    // Clean up after each test
    virtual ~MovesGeneratorTest() {
        // Free resources, if any
    }

};

bool areDequesEqual(const std::deque<Move>& deque1, const std::deque<Move>& deque2) {
    // Check if sizes are equal
    if (deque1.size() != deque2.size()) {
        return false;
    }

    // Make copies of the deques so we can modify them
    std::deque<Move> copy1 = deque1;
    std::deque<Move> copy2 = deque2;
    
    while (!copy1.empty()) {
        auto it = std::find(copy2.begin(), copy2.end(), copy1.front());
        if (it == copy2.end()) {
            // Element not found in copy2
            return false;
        }
        // Erase elements from both copies
        copy1.pop_front();
        copy2.erase(it);
    }

    // If both copies are empty at this point, deques are equal
    return copy1.empty() && copy2.empty();
}


TEST_F(MovesGeneratorTest, PsuedoLegalMovesTest)
{
    board_t board;
    std::deque<std::string> fen = {"position","fen","r3kb1r/1pp2p1p/2nq1n2/1B1pp1pP/p2PPBb1/2N2N2/PPP2PP1/R2QK2R","w","KQkq","g6","0","4"};
    board.init_fen(fen);

    // Moves at this position
    // Moves at this position
    std::deque<Move> actual_moves = {
        // A1 rook
        Move({0,0},{0,1}),
        Move({0,0},{0,2}),
        // queen
        Move({0,3},{0,1}),
        Move({0,3},{0,2}),
        Move({0,3},{1,3}),
        Move({0,3},{1,4}),
        Move({0,3},{2,3}),
        // king
        Move({0,4},{0,5}),
        Move({0,4},{1,3}),
        Move({0,4},{1,4}),
        // H1 rook
        Move({0,7},{0,6}),
        Move({0,7},{0,5}),
        Move({0,7},{1,7}),
        Move({0,7},{2,7}),
        Move({0,7},{3,7}),
        // pawns on 2nd rank
        Move({1,0},{2,0}),
        Move({1,1},{2,1}),
        Move({1,1},{3,1}),
        Move({1,6},{2,6}),
        // nights on 3rd rank
        Move({2,2},{3,0},B_PAWN),
        Move({2,2},{4,3}, B_PAWN),
        Move({2,2},{1,4}),
        Move({2,2},{0,1}),
        Move({2,5},{4,4},B_PAWN),
        Move({2,5},{4,6},B_PAWN),
        Move({2,5},{3,7}),
        Move({2,5},{1,7}),
        Move({2,5},{0,6}),
        Move({2,5},{1,3}),
        // PAWNS on 4th rank
        Move({3,3},{4,4}, B_PAWN),
        Move({3,4},{4,3}, B_PAWN),
        // bihsop on 4th rank
        Move({3,5},{2,4}),
        Move({3,5},{1,3}),
        Move({3,5},{0,2}),
        Move({3,5},{2,6}),
        Move({3,5},{1,7}),
        Move({3,5},{4,6}, B_PAWN),
        Move({3,5},{4,4}, B_PAWN),
        // bihsop on 5th rank
        Move({4,1},{5,0}),
        Move({4,1},{3,2}),
        Move({4,1},{2,3}),
        Move({4,1},{1,4}),
        Move({4,1},{0,5}),
        Move({4,1},{3,0}, B_PAWN),
        Move({4,1},{5,2}, B_KNIGHT),
        // pawn on 5th rank
        Move({4,7},{5,6},B_PAWN, true),
        Move({4,7},{5,7})
    };


    std::deque<Move> generated_moves = board.get_psuedo_legal_moves();

    EXPECT_TRUE(areDequesEqual(actual_moves, generated_moves));
}