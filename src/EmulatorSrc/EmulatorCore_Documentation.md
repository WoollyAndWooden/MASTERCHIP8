# Emulator Core Documentation

The core emulation logic is decoupled from the frontend, allowing it to be used in both the Qt desktop application and the Python AI module.

## Components

*   [SuperChip8](SuperChip8.cpp) - The main CPU / Facade class.
*   [Memory](Memory.md) - RAM management and font loading.
*   [Registers](Registers.md) - CPU registers (V, I, PC, SP) and timers.
*   [Display](Display.md) - 64x32 pixel buffer and drawing logic.
*   [Keyboard](Keyboard.md) - 16-key hex keypad state.

## Architecture

The `SuperChip8` class acts as the central controller. It owns instances of Memory, Registers, Display, and Keyboard. It executes the fetch-decode-execute cycle via the `Tick()` method.

## Flowcharts

### CPU Cycle (Tick)

```mermaid
graph TD
    Start((Start Tick)) --> Loop{Cycles < Speed?}
    Loop -->|Yes| Fetch[Fetch Opcode at PC]
    Loop -->|No| UpdateTimers[Update Delay/Sound Timers]
    
    Fetch --> Increment[Increment PC += 2]
    Increment --> Decode{Decode Opcode}
    
    Decode -->|00E0| CLS[Clear Screen]
    Decode -->|1NNN| Jump[Jump to NNN]
    Decode -->|6XNN| SetV[Set Vx = NN]
    Decode -->|7XNN| AddV[Add NN to Vx]
    Decode -->|DXYN| Draw[Draw Sprite]
    Decode -->|...| Other[Other Instructions]
    
    CLS --> Loop
    Jump --> Loop
    SetV --> Loop
    AddV --> Loop
    Draw --> Loop
    Other --> Loop
    
    UpdateTimers --> End((End Tick))
```

### Drawing Logic (DXYN)

```mermaid
graph TD
    Start((Start DXYN)) --> Init[Set VF = 0]
    Init --> RowLoop{Row < N?}
    
    RowLoop -->|No| End((End))
    RowLoop -->|Yes| FetchByte[Read Byte at I + Row]
    FetchByte --> BitLoop{Bit < 8?}
    
    BitLoop -->|No| NextRow[Row++]
    NextRow --> RowLoop
    
    BitLoop -->|Yes| CheckBit{Is Bit Set?}
    CheckBit -->|No| NextBit[Bit++]
    NextBit --> BitLoop
    
    CheckBit -->|Yes| XOR[XOR Display Pixel]
    XOR --> CheckCollision{Pixel Erased?}
    
    CheckCollision -->|Yes| SetVF[Set VF = 1]
    CheckCollision -->|No| NextBit
    SetVF --> NextBit
```
