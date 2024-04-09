#include <gtest/gtest.h>
#include "lib/board.h"

namespace {
    bool tt() { return true;}
}

TEST(CHECK_GTEST, SAMPLE_TEST)
{
    EXPECT_TRUE(tt());
}