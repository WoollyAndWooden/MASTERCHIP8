# Memory

The `Memory` class manages the 4KB (4096 bytes) of RAM.

## Layout

*   **0x000 - 0x1FF**: Reserved for interpreter (font data stored here).
*   **0x200 - 0xFFF**: Program / ROM space.

## Features

*   **Font Loading**: Automatically loads the standard hexadecimal font set into 0x050-0x09F on initialization.
*   **Opcode Reading**: Reads two bytes and combines them into a 16-bit opcode.

## API

*   `read_byte(addr)`: Returns the byte at the given address.
*   `write_to_memory(addr, value)`: Writes a byte to the given address.
*   `read_opcode(pc)`: Reads the 16-bit opcode at the program counter.
