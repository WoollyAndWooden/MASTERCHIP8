//
// Created by Gaming on 21/01/2026.
//

#pragma once
#include <array>
#include <cstdint>

namespace SuperChip8
{
    class Keyboard
    {
    public:
        Keyboard();
        void press_key(uint8_t key);
        void release_key(uint8_t key);
        bool flip_key(uint8_t key);
        bool is_pressed(uint8_t key);
    private:
        std::array<bool, 16> key_state = std::array<bool, 16>();
    };
} // SuperChip8