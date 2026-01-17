#include <gtest/gtest.h>

// A simple dummy test to verify GTest is working
TEST(SanityCheck, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
