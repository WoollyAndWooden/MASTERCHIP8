#pragma once

#include<array>
#include <cstdint>
#include <string>

namespace SuperChip8
{
    class Memory
    {
    public:
        Memory();
        uint16_t read_opcode(uint16_t address);
        uint8_t read_byte(uint16_t address);
        void write_to_memory(uint16_t address, uint8_t value);
    private:
        std::array<uint8_t, 4096> memory;
    };
}
