#include "Registers.h"

namespace SuperChip8
{
    Registers::Registers() = default;
    uint16_t Registers::get_PC()
    {
        return PC;
    }
    void Registers::set_PC(uint16_t value)
    {
        PC = value;
    }

    void Registers::increase_PC()
    {
        PC += 2;
    }

    void Registers::decrease_PC()
    {
        PC -= 2;
    }

    uint8_t Registers::get_V(uint8_t index)
    {
        return V[index];
    }
    void Registers::set_V(uint8_t index, uint8_t value)
    {
        V[index] = value;
    }
    uint16_t Registers::get_I()
    {
        return I;
    }
    void Registers::set_I(uint16_t value)
    {
        I = value;
    }

    uint16_t Registers::pop_stack()
    {
        uint16_t ret = stack.top();
        stack.pop();
        return ret;
    }
    void Registers::push_stack(uint16_t value)
    {
        stack.push(value);
    }

    uint8_t Registers::get_delay_timer()
    {
        return delay_timer;
    }
    void Registers::set_delay_timer(uint8_t value)
    {
        delay_timer = value;
    }
    void Registers::decrease_delay_timer()
    {
        if (delay_timer > 0)
            delay_timer--;
    }
    uint8_t Registers::get_sound_timer()
    {
        return sound_timer;
    }
    void Registers::set_sound_timer(uint8_t value)
    {
        sound_timer = value;
    }

    void Registers::decrease_sound_timer()
    {
        if (sound_timer > 0)
            sound_timer--;
    }
} // SuperChip8
