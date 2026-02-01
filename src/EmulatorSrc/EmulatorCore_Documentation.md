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
    Increment --> Decode[[Decode Opcode]]
    
    Decode --> Execute[Execute Instruction]
    
    Execute --> Loop
    
    
    UpdateTimers --> End((End Tick))
```

### Jumptable Logic

This emulator uses precompiled jump tables, Decoding is split into smaller chunks, and is directed by table of function pointers
to the proper instruction. This graph takes place between decode and execute from chart above.

```mermaid
graph TD
    Start((Opcode)) --> Get_first{First Nibble}
    
    Get_first -->|0x0| Set_0{Grab last Nibble}
    
    Get_first -->|0x1| SET_PC((Set PC to NNN))
    
    Get_first -->|0x2| PushStack[Push PC to Stack]
    PushStack --> SET_PC
    
    Get_first -->|0x3| VXeqNN{VX == NN}
    
    VXeqNN -->|Yes| Increase_PC((Skip next instruction))
    VXeqNN --> |No| End((Do nothing))
    
    Get_first -->|0x4| VXneNN{VX != NN}
    VXneNN -->|Yes| Increase_PC
    VXneNN --> |No| End
    
    Get_first -->|0x5| VXeqVY{VX==VY}
    VXeqVY -->|Yes| Increase_PC
    VXeqVY --> |No| End
    
    Get_first -->|0x6| SET_VX((Set VX to NN))
    
    Get_first -->|0x7| ADD_VX((Add NN to VX))
    
    Get_first -->|0x8| Set_8{Grab last Nibble}
    
    Set_0 -->|0x00E0| CLS((Clear Screen))
    Set_0 -->|0x00EE| RET((Return from Subroutine))
    
    Set_8 -->|0x8XY0| SETXY((VX = VY))
    
    Set_8 -->|0x8XY1| ORXY((VX OR VY))
    
    Set_8 -->|0x8XY2| ANDXY((VX AND VY))
    
    Set_8 -->|0x8XY3| XORXY((VX XOR VY))
    
    Set_8 -->|0x8XY4| ADDXY[sum = VX+VY]
    ADDXY --> IsOverflow{sum > 0xFF?}
    IsOverflow -->|Yes| SetVF[VF = 1]
    IsOverflow -->|No| SetVX((VX = sum % 0xFF))
    SetVF --> SetVX
    
    Set_8 -->|0x8XY5| IsOverflowSub{VX >= VY?}
    IsOverflowSub -->|Yes| prepareVF[(1)]
    IsOverflowSub -->|No| prepareVF[(0)]
    
    
    
    

    
    
    
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
