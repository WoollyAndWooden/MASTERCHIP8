#include "SuperChip8.h"
#include <fstream>
#include <vector>

namespace SuperChip8
{
    SuperChip8::SuperChip8(std::string filename)
    {
        display = Display();
        memory = Memory();
        registers = Registers();

        // 1. Initialize all tables with OP_NULL for safety
        jumptable_first.fill(&SuperChip8::OP_NULL);
        jumptable_0.fill(&SuperChip8::OP_NULL);
        jumptable_8.fill(&SuperChip8::OP_NULL);
        jumptable_E.fill(&SuperChip8::OP_NULL);
        jumptable_F.fill(&SuperChip8::OP_NULL);
        jumptable_F_Y__.fill(&SuperChip8::OP_NULL);

        // 2. Main Table (First Nibble)
        jumptable_first[0x0] = &SuperChip8::set_0;
        jumptable_first[0x1] = &SuperChip8::set_1;
        jumptable_first[0x2] = &SuperChip8::set_2;
        jumptable_first[0x3] = &SuperChip8::set_3;
        jumptable_first[0x4] = &SuperChip8::set_4;
        jumptable_first[0x5] = &SuperChip8::set_5;
        jumptable_first[0x6] = &SuperChip8::set_6;
        jumptable_first[0x7] = &SuperChip8::set_7;
        jumptable_first[0x8] = &SuperChip8::set_8;
        jumptable_first[0x9] = &SuperChip8::set_9;
        jumptable_first[0xA] = &SuperChip8::set_A;
        jumptable_first[0xB] = &SuperChip8::set_B;
        jumptable_first[0xC] = &SuperChip8::set_C;
        jumptable_first[0xD] = &SuperChip8::set_D;
        jumptable_first[0xE] = &SuperChip8::set_E;
        jumptable_first[0xF] = &SuperChip8::set_F;

        // 3. Table 0 (00E0, 00EE) - Indexed by last nibble
        jumptable_0[0x0] = &SuperChip8::cls_00E0;
        jumptable_0[0xE] = &SuperChip8::ret_00EE;

        // 4. Table 8 (8XYN) - Indexed by last nibble
        jumptable_8[0x0] = &SuperChip8::set_8XY0;
        jumptable_8[0x1] = &SuperChip8::or_8XY1;
        jumptable_8[0x2] = &SuperChip8::and_8XY2;
        jumptable_8[0x3] = &SuperChip8::xor_8XY3;
        jumptable_8[0x4] = &SuperChip8::add_8XY4;
        jumptable_8[0x5] = &SuperChip8::sub_8XY5;
        jumptable_8[0x6] = &SuperChip8::shift_8XY6;
        jumptable_8[0x7] = &SuperChip8::subn_8XY7;
        jumptable_8[0xE] = &SuperChip8::shift_8XYE;

        // 5. Table E (EX9E, EXA1) - Indexed by last nibble
        jumptable_E[0xE] = &SuperChip8::skip_EX9E;
        jumptable_E[0x1] = &SuperChip8::skip_EXA1;

        // 6. Table F (FXNN) - Indexed by last nibble
        jumptable_F[0x7] = &SuperChip8::set_FX07;
        jumptable_F[0xA] = &SuperChip8::get_key_FX0A;
        jumptable_F[0x5] = &SuperChip8::set_FXY5; // Handles 15, 55, 65 via sub-table
        jumptable_F[0x8] = &SuperChip8::set_FX18;
        jumptable_F[0xE] = &SuperChip8::add_to_I_FX1E;
        jumptable_F[0x9] = &SuperChip8::font_FX29;
        jumptable_F[0x3] = &SuperChip8::decimal_conv_FX33; // FX33

        // 7. Table F_Y__ (Helper for FX15, FX55, FX65) - Indexed by (Y)
        jumptable_F_Y__[0x1] = &SuperChip8::set_FX15;
        jumptable_F_Y__[0x5] = &SuperChip8::store_FX55;
        jumptable_F_Y__[0x6] = &SuperChip8::load_FX65;

        // Load ROM
        std::ifstream ROM(filename, std::ios::binary | std::ios::ate);
        if (ROM.is_open())
        {
            std::streampos size = ROM.tellg();
            ROM.seekg(0, std::ios::beg);
            std::vector<char> buffer(size);

            if (ROM.read(buffer.data(), size))
                for (int i = 0; i < size; i++)
                    if (0x200 + i < 4096) // All programs are loaded at 0x200
                        memory.write_to_memory(0x200 + i, static_cast<uint8_t>(buffer[i]));
        }
    }

    void SuperChip8::Cycle()
    {
        opcode = memory.read_opcode(registers.get_PC());

        registers.set_PC(registers.get_PC() + 2);

        (this->*jumptable_first[get_first()])();
    }

    void SuperChip8::Tick()
    {
        for (int i = 0; i < cycles_per_tick; i++)
            Cycle();
        registers.decrease_delay_timer();
        registers.decrease_sound_timer();
    }

    void SuperChip8::change_cycles_per_tick(uint8_t value)
    {
        cycles_per_tick = value;
    }

    void SuperChip8::SCHIP8MODE_switch(bool schip8mode)
    {
        SCHIP8MODE = schip8mode;
    }

    // --- Opcode Implementations ---

    void SuperChip8::set_0()
    {
        (this->*jumptable_0[get_N()])();
    }
    void SuperChip8::cls_00E0()
    {
        for (int i = 0; i < 128; i++)
            for (int j = 0; j < 64; j++)
                display.set_pixel(i, j, false);
    }
    void SuperChip8::ret_00EE() {}

    void SuperChip8::set_1()
    {
        registers.set_PC(get_NNN());
    }
    void SuperChip8::set_2() {}
    void SuperChip8::set_3() {}
    void SuperChip8::set_4() {}
    void SuperChip8::set_5() {}
    void SuperChip8::set_6()
    {
        registers.set_V(get_X(), get_NN());
    }
    void SuperChip8::set_7()
    {
        registers.set_V(get_X(), registers.get_V(get_X()) + get_NN());
    }

    void SuperChip8::set_8()
    {
        (this->*jumptable_8[get_N()])();
    }
    void SuperChip8::set_8XY0() {}
    void SuperChip8::or_8XY1() {}
    void SuperChip8::and_8XY2() {}
    void SuperChip8::xor_8XY3() {}
    void SuperChip8::add_8XY4() {}
    void SuperChip8::sub_8XY5() {}
    void SuperChip8::shift_8XY6() {}
    void SuperChip8::subn_8XY7() {}
    void SuperChip8::shift_8XYE() {}

    void SuperChip8::set_9() {}
    void SuperChip8::set_A()
    {
        registers.set_I(get_NNN());
    }
    void SuperChip8::set_B() {}
    void SuperChip8::set_C() {}
    void SuperChip8::set_D()
    {
        registers.set_V(0xF, 0);
        uint8_t Y = registers.get_V(get_Y()) % 64;
        bool stop_row = false, stop = false;

        for (int i = 0; i < get_N() && !stop; i++)
        {

            uint8_t X = registers.get_V(get_X()) % 128;
            uint8_t sprite_row = memory.read_byte(registers.get_I() + i);
            stop_row = false;
            for (int j = 0; j < 8 && !stop_row; j++)
            {
                if (sprite_row & (0x80) >> j)
                {
                    bool flipped = display.flipPixel(X, Y);
                    if (flipped)
                        registers.set_V(0xF, 1);

                }
                if (X == 127)
                    stop_row = true;
                X++;
            }
            if (Y == 63)
                stop = true;
            Y++;
        }
    }

    void SuperChip8::set_E()
    {
        (this->*jumptable_E[get_N()])();
    }
    void SuperChip8::skip_EX9E() {}
    void SuperChip8::skip_EXA1() {}

    void SuperChip8::set_F()
    {
        (this->*jumptable_F[get_N()])();
    }
    void SuperChip8::decimal_conv_FX33() {}
    void SuperChip8::set_FX07() {}
    void SuperChip8::set_FXY5()
    {
        (this->*jumptable_F_Y__[get_Y()])();
    }
    void SuperChip8::set_FX15() {}
    void SuperChip8::store_FX55() {}
    void SuperChip8::load_FX65() {}
    void SuperChip8::set_FX18() {}
    void SuperChip8::font_FX29() {}
    void SuperChip8::add_to_I_FX1E() {}
    void SuperChip8::get_key_FX0A() {}

    void SuperChip8::OP_NULL() {}

} // SuperChip8
