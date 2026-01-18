#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "SuperChip8.h"

class SuperChip8Test : public ::testing::Test {
protected:
    std::string tempRomName = "test_rom.ch8";

    void SetUp() override {
        std::ofstream rom(tempRomName, std::ios::binary);
        uint8_t data[] = {0xA1, 0xB2, 0xC3};
        rom.write(reinterpret_cast<char*>(data), sizeof(data));
        rom.close();
    }

    void TearDown() override {
        std::remove(tempRomName.c_str());
    }
};

TEST_F(SuperChip8Test, LoadRomToMemory) {
    SuperChip8::SuperChip8 cpu(tempRomName);
    EXPECT_EQ(cpu.memory.read_byte(0x200), 0xA1);
    EXPECT_EQ(cpu.memory.read_byte(0x201), 0xB2);
    EXPECT_EQ(cpu.memory.read_byte(0x202), 0xC3);
}

TEST_F(SuperChip8Test, DisplayAccess) {
    SuperChip8::SuperChip8 cpu(tempRomName);
    cpu.display.set_pixel(10, 10, true);
    EXPECT_TRUE(cpu.display.getPixel(10, 10));
}

TEST_F(SuperChip8Test, TickDecrementsTimers) {
    SuperChip8::SuperChip8 cpu(tempRomName);
    cpu.registers.set_delay_timer(5);
    cpu.registers.set_sound_timer(10);
    cpu.Tick();
    EXPECT_EQ(cpu.registers.get_delay_timer(), 4);
    EXPECT_EQ(cpu.registers.get_sound_timer(), 9);
}

TEST_F(SuperChip8Test, CyclesPerTickConfig) {
    SuperChip8::SuperChip8 cpu(tempRomName);
    cpu.change_cycles_per_tick(20);
    cpu.Tick();
    SUCCEED();
}

TEST_F(SuperChip8Test, FetchCycleIncrementsPC) {
    SuperChip8::SuperChip8 cpu(tempRomName);

    // Initial PC is 0x200
    EXPECT_EQ(cpu.registers.get_PC(), 0x200);

    // Run one cycle
    cpu.Cycle();

    // PC should increment by 2
    EXPECT_EQ(cpu.registers.get_PC(), 0x202);
}

// --- BCD Tests (FX33) ---

TEST_F(SuperChip8Test, BCD_Conversion_123) {
    SuperChip8::SuperChip8 cpu(tempRomName);

    // Setup: V0 = 123, I = 0x300
    cpu.registers.set_V(0, 123);
    cpu.registers.set_I(0x300);

    // Opcode: F033 (Store BCD of V0 at I)
    // Write opcode to memory at current PC (0x200)
    cpu.memory.write_to_memory(0x200, 0xF0);
    cpu.memory.write_to_memory(0x201, 0x33);

    cpu.Cycle();

    // Check memory
    EXPECT_EQ(cpu.memory.read_byte(0x300), 1); // Hundreds
    EXPECT_EQ(cpu.memory.read_byte(0x301), 2); // Tens
    EXPECT_EQ(cpu.memory.read_byte(0x302), 3); // Ones
}

TEST_F(SuperChip8Test, BCD_Conversion_255) {
    SuperChip8::SuperChip8 cpu(tempRomName);

    cpu.registers.set_V(1, 255);
    cpu.registers.set_I(0x400);

    // Opcode: F133 (Store BCD of V1 at I)
    cpu.memory.write_to_memory(0x200, 0xF1);
    cpu.memory.write_to_memory(0x201, 0x33);

    cpu.Cycle();

    EXPECT_EQ(cpu.memory.read_byte(0x400), 2);
    EXPECT_EQ(cpu.memory.read_byte(0x401), 5);
    EXPECT_EQ(cpu.memory.read_byte(0x402), 5);
}

TEST_F(SuperChip8Test, BCD_Conversion_0) {
    SuperChip8::SuperChip8 cpu(tempRomName);

    cpu.registers.set_V(2, 0);
    cpu.registers.set_I(0x500);

    // Opcode: F233
    cpu.memory.write_to_memory(0x200, 0xF2);
    cpu.memory.write_to_memory(0x201, 0x33);

    cpu.Cycle();

    EXPECT_EQ(cpu.memory.read_byte(0x500), 0);
    EXPECT_EQ(cpu.memory.read_byte(0x501), 0);
    EXPECT_EQ(cpu.memory.read_byte(0x502), 0);
}

TEST_F(SuperChip8Test, BCD_Conversion_5) {
    SuperChip8::SuperChip8 cpu(tempRomName);

    cpu.registers.set_V(3, 5);
    cpu.registers.set_I(0x600);

    // Opcode: F333
    cpu.memory.write_to_memory(0x200, 0xF3);
    cpu.memory.write_to_memory(0x201, 0x33);

    cpu.Cycle();

    EXPECT_EQ(cpu.memory.read_byte(0x600), 0);
    EXPECT_EQ(cpu.memory.read_byte(0x601), 0);
    EXPECT_EQ(cpu.memory.read_byte(0x602), 5);
}
