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
