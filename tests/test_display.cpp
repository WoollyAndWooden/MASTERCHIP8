#include <gtest/gtest.h>
#include "Display.h"

class DisplayTest : public ::testing::Test {
protected:
    SuperChip8::Display display;
};

// Test 1: Basic Toggling (flipPixel)
TEST_F(DisplayTest, PixelToggling) {
    int x = 10;
    int y = 10;

    // Initial state should be false
    EXPECT_FALSE(display.getPixel(x, y));

    // First toggle: false -> true
    bool state1 = display.flipPixel(x, y);
    EXPECT_TRUE(state1);
    EXPECT_TRUE(display.getPixel(x, y));

    // Second toggle: true -> false
    bool state2 = display.flipPixel(x, y);
    EXPECT_FALSE(state2);
    EXPECT_FALSE(display.getPixel(x, y));
}

// Test 2: Explicit Setting (set_pixel)
TEST_F(DisplayTest, SetPixel) {
    int x = 5;
    int y = 5;

    // Set to true
    display.set_pixel(x, y, true);
    EXPECT_TRUE(display.getPixel(x, y));

    // Set to false
    display.set_pixel(x, y, false);
    EXPECT_FALSE(display.getPixel(x, y));

    // Set to true again to verify it sticks
    display.set_pixel(x, y, true);
    EXPECT_TRUE(display.getPixel(x, y));
}

// Test 3: Independence of Pixels
TEST_F(DisplayTest, PixelIndependence) {
    // Set (10, 10) to true
    display.set_pixel(10, 10, true);
    EXPECT_TRUE(display.getPixel(10, 10));

    // Set (20, 20) to true
    display.set_pixel(20, 20, true);
    EXPECT_TRUE(display.getPixel(20, 20));

    // Flip (10, 10) back to false
    display.flipPixel(10, 10);
    EXPECT_FALSE(display.getPixel(10, 10));

    // (20, 20) should still be true
    EXPECT_TRUE(display.getPixel(20, 20));
}

// Test 4: Default Initialization
TEST_F(DisplayTest, DefaultInitialization) {
    // Verify a few random pixels are false initially
    // Updated for 64x32 resolution
    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(63, 31)); // Max valid coordinate
    EXPECT_FALSE(display.getPixel(32, 16)); // Center
}
