/**
 * @file test_sanity.cpp
 * @brief Sanity check test to verify Google Test is working
 * @date 2025-11-12
 * 
 * This file contains basic tests to ensure the testing infrastructure
 * is properly set up and working.
 */

#include <gtest/gtest.h>

/**
 * Basic sanity test - always passes
 * Verifies that Google Test framework is functioning
 */
TEST(SanityTest, GoogleTestWorks) {
    EXPECT_TRUE(true);
    EXPECT_EQ(1 + 1, 2);
}

/**
 * Test integer arithmetic
 */
TEST(SanityTest, IntegerArithmetic) {
    int a = 5;
    int b = 3;
    
    EXPECT_EQ(a + b, 8);
    EXPECT_EQ(a - b, 2);
    EXPECT_EQ(a * b, 15);
}

/**
 * Test floating point comparisons
 */
TEST(SanityTest, FloatingPointComparison) {
    float a = 0.1f + 0.2f;
    float b = 0.3f;
    
    // Use FLOAT_EQ for floating point comparisons
    EXPECT_FLOAT_EQ(a, b);
}

/**
 * Test string comparisons
 */
TEST(SanityTest, StringComparison) {
    std::string str1 = "Hello";
    std::string str2 = "Hello";
    std::string str3 = "World";
    
    EXPECT_EQ(str1, str2);
    EXPECT_NE(str1, str3);
    EXPECT_STREQ(str1.c_str(), "Hello");
}

/**
 * Test boolean conditions
 */
TEST(SanityTest, BooleanConditions) {
    bool flag = true;
    
    EXPECT_TRUE(flag);
    EXPECT_FALSE(!flag);
}

/**
 * Test null pointer checks
 */
TEST(SanityTest, NullPointerCheck) {
    int* ptr = nullptr;
    int value = 42;
    int* validPtr = &value;
    
    EXPECT_EQ(ptr, nullptr);
    EXPECT_NE(validPtr, nullptr);
}

/**
 * Fixture example for grouped tests
 */
class SanityFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Called before each test
        testValue = 100;
    }
    
    void TearDown() override {
        // Called after each test
    }
    
    int testValue;
};

TEST_F(SanityFixture, FixtureSetupWorks) {
    EXPECT_EQ(testValue, 100);
}

TEST_F(SanityFixture, FixtureModificationIsolated) {
    testValue = 200;
    EXPECT_EQ(testValue, 200);
    // Next test will have testValue = 100 again due to SetUp
}

/**
 * Main function - not needed if using GTest::Main,
 * but shown here for completeness
 */
// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
