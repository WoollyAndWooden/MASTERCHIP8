#include <gtest/gtest.h>
#include "Memory.h"

class MemoryTest : public ::testing::Test {
protected:
    SuperChip8::Memory memory;
};

// Test 1: Verify Font Loading
// Fonts should be loaded at 0x050 (80 decimal)
TEST_F(MemoryTest, FontInitialization) {
    // Check '0' font (0xF0, 0x90, 0x90, 0x90, 0xF0) at 0x050
    EXPECT_EQ(memory.read_byte(0x50), 0xF0);
    EXPECT_EQ(memory.read_byte(0x51), 0x90);
    EXPECT_EQ(memory.read_byte(0x54), 0xF0);

    // Check 'F' font (0xF0, 0x80, 0xF0, 0x80, 0x80) at 0x09F (last byte)
    EXPECT_EQ(memory.read_byte(0x9F), 0x80);
}

// Test 2: Read and Write
TEST_F(MemoryTest, ReadWrite) {
    uint16_t testAddress = 0x200; // Program start
    uint8_t testValue = 0x42;

    memory.write_to_memory(testAddress, testValue);
    EXPECT_EQ(memory.read_byte(testAddress), testValue);
}

// Test 3: Default Initialization (Zeroes)
TEST_F(MemoryTest, DefaultInitialization) {
    EXPECT_EQ(memory.read_byte(0x00), 0x00);
    EXPECT_EQ(memory.read_byte(0x10), 0x00);
}

// Test 4: Read Opcode (Word)
TEST_F(MemoryTest, ReadOpcode) {
    // Write 0xAB at 0x200 and 0xCD at 0x201
    memory.write_to_memory(0x200, 0xAB);
    memory.write_to_memory(0x201, 0xCD);

    // Should combine to 0xABCD
    EXPECT_EQ(memory.read_opcode(0x200), 0xABCD);
}
