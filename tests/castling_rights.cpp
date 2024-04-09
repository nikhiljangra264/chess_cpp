#include <gtest/gtest.h>
#include "../src/lib/board.h"

// Define a fixture for your tests
class CastlingRightsTest : public ::testing::Test {
protected:
    // Set up common variables or objects needed for your tests
    CastlingRightsTest() {
        // Initialize anything needed before each test
    }

    // Clean up after each test
    virtual ~CastlingRightsTest() {
        // Free resources, if any
    }

};

TEST_F(CastlingRightsTest, set_function_test) {
    castling_rights rights(0);

    // Test setting and getting castling rights
    rights.set_wking_castle_queen(true);
    EXPECT_TRUE(rights.get_wking_castle_queen());
    EXPECT_FALSE(rights.get_wking_castle_king());
    EXPECT_FALSE(rights.get_bking_castle_queen());
    EXPECT_FALSE(rights.get_bking_castle_king());

    rights.set_wking_castle_king(true);
    EXPECT_TRUE(rights.get_wking_castle_queen());
    EXPECT_TRUE(rights.get_wking_castle_king());
    EXPECT_FALSE(rights.get_bking_castle_queen());
    EXPECT_FALSE(rights.get_bking_castle_king());

    rights.set_bking_castle_queen(true);
    EXPECT_TRUE(rights.get_wking_castle_queen());
    EXPECT_TRUE(rights.get_wking_castle_king());
    EXPECT_TRUE(rights.get_bking_castle_queen());
    EXPECT_FALSE(rights.get_bking_castle_king());

    rights.set_bking_castle_king(true);
    EXPECT_TRUE(rights.get_wking_castle_queen());
    EXPECT_TRUE(rights.get_wking_castle_king());
    EXPECT_TRUE(rights.get_bking_castle_queen());
    EXPECT_TRUE(rights.get_bking_castle_king());

}

TEST_F(CastlingRightsTest, revoke_function_test)
{
    castling_rights rights;
    // Test revoking castling rights
    rights.revoke_castling_rights(WHITE);
    EXPECT_FALSE(rights.get_wking_castle_queen());
    EXPECT_FALSE(rights.get_wking_castle_king());
    EXPECT_TRUE(rights.get_bking_castle_queen());
    EXPECT_TRUE(rights.get_bking_castle_king());

    rights.revoke_castling_rights(BLACK);
    EXPECT_FALSE(rights.get_wking_castle_queen());
    EXPECT_FALSE(rights.get_wking_castle_king());
    EXPECT_FALSE(rights.get_bking_castle_queen());
    EXPECT_FALSE(rights.get_bking_castle_king());
}
