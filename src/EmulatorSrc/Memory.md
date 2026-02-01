# Memory

The `Memory` class manages the 4KB (4096 bytes) of RAM available to the CHIP-8 system.

## Memory Map

The 4KB of memory is laid out as follows. This implementation follows the standard convention.

| Address Range   | Size      | Purpose                               |
|-----------------|-----------|---------------------------------------|
| `0x000 - 0x1FF` | 512 Bytes | **Interpreter Area**                  |
| `↳ 0x050 - 0x09F` | 80 Bytes  | Internal Font Set (0-F)               |
| `0x200 - 0xFFF` | 3584 Bytes| **Program ROM and RAM**               |
| `↳ 0x200`       | -         | Start of most CHIP-8 programs.        |
| `↳ ...`         | -         | ROM data is loaded here.              |
| `↳ ...`         | -         | The rest is used as RAM by the program.|

*Note: Some original hardware used `0xF00-0xFFF` for the display buffer, but modern emulators typically keep the display buffer separate from main memory for performance.*

## Features

*   **Font Loading**: On initialization, the class automatically loads the standard 5-byte hexadecimal font set into the interpreter area (`0x050`).
*   **Opcode Reading**: Provides a helper function to read two consecutive bytes from memory and combine them into a single 16-bit opcode, handling the necessary bit-shifting.

## API

*   `read_byte(addr)`: Returns the byte at the given address.
*   `write_to_memory(addr, value)`: Writes a byte to the given address.
*   `read_opcode(pc)`: Reads the 16-bit opcode starting at the program counter address.
