# MASTERCHIP8 Python Module API

This document lists the functions exposed by the C++ core to the Python `masterchip8_py` module.

## Exposed Functions

*   `SuperChip8(rom_path: str)`: Constructor. Initializes the emulator with the given ROM file path.
*   `tick()`: Executes one full tick of the emulator (including multiple CPU cycles and timer decrements).
*   `change_cycles_per_tick(cycles: int)`: Sets the number of CPU cycles to run per tick.
*   `press_key(key_index: int)`: Simulates pressing a key (0-15).
*   `release_key(key_index: int)`: Simulates releasing a key (0-15).
*   `get_pixel(x: int, y: int) -> bool`: Returns the state of the pixel at the given coordinates (`True` for on, `False` for off).
*   `get_register_V(x: int) -> int`: Returns the value of the V register at index `x`.
