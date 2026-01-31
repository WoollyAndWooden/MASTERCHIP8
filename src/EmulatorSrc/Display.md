# Display

The `Display` class manages the 32x64 pixel monochrome screen of the CHIP-8.

## Features

*   **Resolution**: 64x32 pixels.
*   **Pixel Storage**: Uses a 2D array of booleans (`true` = ON, `false` = OFF).
*   **XOR Drawing**: Implements the XOR drawing logic required by CHIP-8 sprites.
*   **Collision Detection**: Returns collision status when drawing (used by the CPU to set the VF register).

## API

*   `flipPixel(x, y)`: Toggles the pixel at (x, y). Returns `true` if a collision occurred (pixel was turned OFF).
*   `set_pixel(x, y, state)`: Explicitly sets a pixel state.
*   `getPixel(x, y)`: Returns the current state of a pixel.
