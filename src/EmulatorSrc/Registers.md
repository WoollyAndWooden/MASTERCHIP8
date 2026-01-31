# Registers

The `Registers` class manages the CPU registers and timers.

## Components

*   **V Registers (V0-VF)**: 16 general-purpose 8-bit registers. VF is used for flags (carry, collision).
*   **I Register**: 16-bit index register for memory addressing.
*   **PC (Program Counter)**: 16-bit register pointing to the current instruction.
*   **Stack**: 16-level stack for subroutine calls.
*   **SP (Stack Pointer)**: Points to the top of the stack.
*   **Delay Timer**: Decrements at 60Hz.
*   **Sound Timer**: Decrements at 60Hz; plays sound when > 0.

## API

*   Getters and Setters for all registers.
*   `push_stack(value)` / `pop_stack()`: Manage subroutine calls.
*   `decrease_delay_timer()` / `decrease_sound_timer()`: Called by the CPU tick loop.
