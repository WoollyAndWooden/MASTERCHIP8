# Qt GUI Documentation

The Desktop Application provides a graphical interface for the emulator using the Qt 6 framework.

## Components

*   **MainWindow**: The main application window, handling the menu bar, toolbar, and emulation loop.
*   **EmulatorDisplay**: A custom `QOpenGLWidget` that renders the emulator's display buffer.
*   **SettingsDialog**: A dialog for configuring emulator settings (colors, grid, quirks).

## Flowcharts

### Main Loop

```mermaid
graph TD
    Start((App Start)) --> Init[Initialize MainWindow]
    Init --> LoadROM[/User Loads ROM/]
    
    LoadROM --> CreateEmu[Create Emulator Instance]
    CreateEmu --> StartTimer[Start QTimer at 16ms]
    
    StartTimer --> TimerFires{Timer Fires at 60Hz}
    TimerFires --> Tick[Call emulator->Tick]
    Tick --> UpdateDisplay[Call display->update]
    UpdateDisplay --> UpdateDebug[Update Debug Table]
    UpdateDebug --> CheckSound{Sound Timer > 0?}
    
    CheckSound -->|Yes| PlaySound[Play/Resume Beep]
    CheckSound -->|No| StopSound[Stop/Suspend Beep]
    
    PlaySound --> TimerFires
    StopSound --> TimerFires
```
