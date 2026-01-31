# Keyboard

The `Keyboard` class manages the state of the 16-key hex keypad (0-F).

## Features

*   **State Tracking**: Stores the current state (pressed/released) of all 16 keys.
*   **Mapping**: The mapping from physical keys (Qt/SDL) to CHIP-8 keys is handled by the frontend, not this class.

## API

*   `press_key(key)`: Marks a key (0-15) as pressed.
*   `release_key(key)`: Marks a key (0-15) as released.
*   `is_pressed(key)`: Returns `true` if the key is currently pressed.
