#include <gtest/gtest.h>
#include "Registers.h"

class RegistersTest : public ::testing::Test {
protected:
    SuperChip8::Registers registers;
};

// Test 1: Initialization
TEST_F(RegistersTest, Initialization) {
    EXPECT_EQ(registers.get_PC(), 0x200);
    EXPECT_EQ(registers.get_I(), 0);
    EXPECT_EQ(registers.get_delay_timer(), 0);
    EXPECT_EQ(registers.get_sound_timer(), 0);
}

// Test 2: Program Counter (PC)
TEST_F(RegistersTest, ProgramCounter) {
    registers.set_PC(0xABC);
    EXPECT_EQ(registers.get_PC(), 0xABC);
}

// Test 3: V Registers
TEST_F(RegistersTest, VRegisters) {
    registers.set_V(0, 0x12);
    EXPECT_EQ(registers.get_V(0), 0x12);
    registers.set_V(0xF, 0xFF);
    EXPECT_EQ(registers.get_V(0xF), 0xFF);
    registers.set_V(1, 0x55);
    EXPECT_EQ(registers.get_V(0), 0x12);
}

// Test 4: Index Register (I)
TEST_F(RegistersTest, IndexRegister) {
    registers.set_I(0x1234);
    EXPECT_EQ(registers.get_I(), 0x1234);
}

// Test 5: Stack Operations
TEST_F(RegistersTest, Stack) {
    registers.push_stack(0x111);
    registers.push_stack(0x222);
    EXPECT_EQ(registers.pop_stack(), 0x222);
    EXPECT_EQ(registers.pop_stack(), 0x111);
}

// Test 6: Timers Set/Get
TEST_F(RegistersTest, Timers) {
    registers.set_delay_timer(60);
    EXPECT_EQ(registers.get_delay_timer(), 60);

    registers.set_sound_timer(30);
    EXPECT_EQ(registers.get_sound_timer(), 30);
}

// Test 7: Timer Decrement Logic
TEST_F(RegistersTest, TimerDecrement) {
    registers.set_delay_timer(2);
    registers.decrease_delay_timer();
    EXPECT_EQ(registers.get_delay_timer(), 1);
    registers.decrease_delay_timer();
    EXPECT_EQ(registers.get_delay_timer(), 0);
    registers.decrease_delay_timer(); // Should stay at 0
    EXPECT_EQ(registers.get_delay_timer(), 0);

    registers.set_sound_timer(1);
    registers.decrease_sound_timer();
    EXPECT_EQ(registers.get_sound_timer(), 0);
}
