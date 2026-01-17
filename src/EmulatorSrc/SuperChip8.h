#pragma once
#include <string>

#include "Display.h"
#include "Memory.h"
#include "Registers.h"

namespace SuperChip8
{
    class SuperChip8
    {
    public:
        SuperChip8(std::string filename);

        void Cycle();
        void Tick();

        void change_cycles_per_tick(uint8_t value);

        void SCHIP8MODE_switch(bool schip8mode);

        Display display;
        Memory memory;
        Registers registers;

    private:
        uint8_t cycles_per_tick = 10;

        uint16_t opcode;

        //Emulator mode
        bool SCHIP8MODE = false;

        //Functions to grab required bits from opcode
        uint8_t get_first() const {return (opcode & 0xF000) >> 12;}
        uint8_t get_X() const {return (opcode & 0x0F00) >> 8;}
        uint8_t get_Y() const {return (opcode & 0x00F0) >> 4;}
        uint8_t get_N() const {return opcode & 0x000F;}
        uint8_t get_NN() const {return opcode & 0x00FF;}
        uint16_t get_NNN() const {return opcode & 0x0FFF;}

        using opcode_handler = void (SuperChip8::*)();

        std::array<opcode_handler, 16> jumptable_first = std::array<opcode_handler, 16>();
        std::array<opcode_handler, 16> jumptable_0 = std::array<opcode_handler, 16>();
        std::array<opcode_handler, 16> jumptable_8 = std::array<opcode_handler, 16>();
        std::array<opcode_handler, 16> jumptable_E = std::array<opcode_handler, 16>();
        std::array<opcode_handler, 16> jumptable_F = std::array<opcode_handler, 16>();
        std::array<opcode_handler, 16> jumptable_F_Y__ = std::array<opcode_handler, 16>();


        //CPU instruction implementations
        //First, I need a main set for jumptable. All of those will be accessible by the first nibble of the opcode.
        //0x0 will only clear screen and return from a subroutine
        void set_0();
        void cls_00E0();
        void ret_00EE();

        //0x1 has only jump, no new jumptable
        void set_1();

        //0x2 manages subroutine calls alongside 0x00EE
        void set_2();

        //0x3 through 0x5, a conditional skips
        void set_3();
        void set_4();
        void set_5();

        //0x6 is set
        void set_6();

        //0x7 is add
        void set_7();

        //0x8 is a set of arythmetical and logical operations
        void set_8();
        void set_8XY0();
        void or_8XY1();
        void and_8XY2();
        void xor_8XY3();
        void add_8XY4();
        void sub_8XY5();
        void shift_8XY6();
        void subn_8XY7();
        void shift_8XYE();

        //conditional skips part 2
        void set_9();

        //0xA is set I
        void set_A();

        //0xB is jump with offset
        void set_B();

        //0xC is random
        void set_C();

        //0xD is draw
        void set_D();

        //0xE is conditional skips
        void set_E();
        void skip_EX9E();
        void skip_EXA1();

        //0xF is timer managment and misc
        void set_F();
        void decimal_conv_FX33();
        void set_FX07();

        //those are kinda weird
        void set_FXY5();
        void set_FX15();
        void store_FX55();
        void load_FX65();

        void set_FX18();
        void font_FX29();
        void add_to_I_FX1E();
        void get_key_FX0A();

        void OP_NULL();

    };
} // SuperChip8