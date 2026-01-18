#pragma once
#include <array>
#include <cstdint>
#include <stack>
#include <vector>

namespace SuperChip8
{
    class Registers
    {
    public:
        Registers();
        uint16_t get_PC();
        void set_PC(uint16_t value);
        void increase_PC();
        uint8_t get_V(uint8_t index);
        void set_V(uint8_t index, uint8_t value);
        uint16_t get_I();
        void set_I(uint16_t value);
        uint16_t pop_stack();
        void push_stack(uint16_t value);
        uint8_t get_delay_timer();
        void set_delay_timer(uint8_t value);
        void decrease_delay_timer();
        uint8_t get_sound_timer();
        void set_sound_timer(uint8_t value);
        void decrease_sound_timer();

    private:
        uint16_t PC = 0x200;
        std::array<uint8_t, 16> V = std::array<uint8_t, 16>();
        uint16_t I = 0;
        std::stack<uint16_t> stack = std::stack<uint16_t>();
        uint8_t delay_timer = 0;
        uint8_t sound_timer = 0;
    };
};