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
    
    Get_first -->|0x8| Set_8[[Execute Arithmetic operation]]
    Set_8 --> SetVXToResult((Store result in VX))
    
    Set_0 -->|0x00E0| CLS((Clear Screen))
    Set_0 -->|0x00EE| RET((Return from Subroutine))
    
    Get_first -->|0x9| VXneVY{VX != VY}
    VXneVY -->|Yes| Increase_PC
    VXneVY -->|No| End
    
    Get_first -->|0xA| SET_I((Set I = NNN))
    
    Get_first -->|0xB| JUMP_OFFSET((PC = NNN + V0))
    
    Get_first -->|0xC| RND((VX = rand & NN))
    
    Get_first -->|0xD| DRAW((Draw Sprite))
    
    Get_first -->|0xE| Set_E{Grab last Nibble}
    Set_E -->|0xEX9E| SkipIfPressed{Key Vx Pressed?}
    SkipIfPressed -->|Yes| Increase_PC
    SkipIfPressed -->|No| End
    
    Set_E -->|0xEXA1| SkipIfNotPressed{Key Vx NOT Pressed?}
    SkipIfNotPressed -->|Yes| Increase_PC
    SkipIfNotPressed -->|No| End
    
    Get_first -->|0xF| Set_F[[Execute Special operation]]
```

#### Arithmetic operations 0x8XYN
```mermaid
graph TD

Start((Grab last Nibble)) --> Set_8{0x8XYN}
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
IsOverflowSub -->|Yes| prepareVF1[(prepareVF = 1)]
IsOverflowSub -->|No| prepareVF0[(prepareVF = 0)]
prepareVF0 --> SUBXY[VX -= VY % 255]
prepareVF1 --> SUBXY
SUBXY --> SetVFtoPrepare((VF = prepareVF))

Set_8 -->|0x8XY6| ShiftRight[VF = VX & 1]
ShiftRight --> ShiftRightOp((VX = VX >> 1))

Set_8 -->|0x8XY7| IsOverflowSubN{VY >= VX?}
IsOverflowSubN -->|Yes| prepareVF1N[(prepareVF = 1)]
IsOverflowSubN -->|No| prepareVF0N[(prepareVF = 0)]
prepareVF0N --> SUBNXY[VX = VY - VX]
prepareVF1N --> SUBNXY
SUBNXY --> SetVFtoPrepareN((VF = prepareVF))

Set_8 -->|0x8XYE| ShiftLeft[VF = VX >> 7]
ShiftLeft --> ShiftLeftOp((VX = VX << 1))
```

#### Special Operations 0xFXNN
```mermaid
graph TD
    Start((0xFXNN)) --> CheckNN{Opcode?}
    
    CheckNN -->|0xFX07| GetDelay((VX = Delay Timer))
    
    CheckNN -->|0xFX0A| WasExecuted{Was key previously pressed}
    
    WasExecuted -->|Yes| LoadKey[Load previously pressed key]
    LoadKey --> IsReleased{Is the key released}
    WasExecuted -->|No| ScanKeyboard{Is any key pressed}
    
    IsReleased -->|Yes| SetKeyVX((VX = Key))
    IsReleased -->|No| StoreKey[Store key for next cycle]
    StoreKey --> DecreasePC((Decrease PC))
    ScanKeyboard -->|Yes| IsReleased
    ScanKeyboard -->|No| DecreasePC
    
    CheckNN -->|0xFX15| SetDelay((Delay Timer = VX))
    
    CheckNN -->|0xFX18| SetSound((Sound Timer = VX))
    
    CheckNN -->|0xFX1E| AddToI((I = I + VX))
    
    CheckNN -->|0xFX29| SetFont((I = Font Address for VX))
    
    CheckNN -->|0xFX33| BCD[Calculate BCD of VX]
    BCD --> StoreHundreds[Mem at I = Hundreds]
    StoreHundreds --> StoreTens[Mem at I+1 = Tens]
    StoreTens --> StoreOnes((Mem atI+2  = Ones))
    
    CheckNN -->|0xFX55| StoreLoop{i <= X?}
    StoreLoop -->|Yes| StoreReg[Mem at I+i = VI]
    StoreReg --> IncrementI[i++]
    IncrementI --> StoreLoop
    StoreLoop -->|No| EndStore((End))
    
    CheckNN -->|0xFX65| LoadLoop{i <= X?}
    LoadLoop -->|Yes| LoadReg[VI = Mem at I+i]
    LoadReg --> IncrementL[i++]
    IncrementL --> LoadLoop
    LoadLoop -->|No| EndLoad((End))
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
