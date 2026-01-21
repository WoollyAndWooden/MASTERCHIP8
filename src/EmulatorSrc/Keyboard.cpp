//
// Created by Gaming on 21/01/2026.
//

#include "Keyboard.h"

namespace SuperChip8
{
    Keyboard::Keyboard()
    {
        key_state.fill(false);
    }

    void Keyboard::press_key(uint8_t key)
    {
        key_state[key] = true;
    }

    void Keyboard::release_key(uint8_t key)
    {
        key_state[key] = false;
    }

    bool Keyboard::flip_key(uint8_t key)
    {
        return key_state[key] = !key_state[key];
    }

    bool Keyboard::is_pressed(uint8_t key)
    {
        return key_state[key];

    }
} // SuperChip8