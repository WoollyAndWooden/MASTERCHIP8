# Qt GUI Documentation

The Desktop Application provides a graphical interface for the emulator using the Qt 6 framework.

## Components

*   **MainWindow**: The main application window, handling the menu bar, toolbar, and emulation loop.
*   **EmulatorDisplay**: A custom `QOpenGLWidget` that renders the emulator's display buffer.
*   **SettingsDialog**: A dialog for configuring emulator settings (colors, grid, quirks).

## Flowcharts

### Main Loop
*(Placeholder: Insert Flowchart of Qt Timer & Emulation Loop here)*
1.  QTimer fires (60Hz).
2.  Call `emulator->Tick()`.
3.  Call `EmulatorDisplay->update()`.
4.  Update Debug Table.
5.  Check Sound Timer -> Play/Stop Beep.

### Input Handling
*(Placeholder: Insert Flowchart of Key Press Event here)*
1.  User presses key.
2.  Qt `keyPressEvent` captures it.
3.  Map Qt Key to CHIP-8 Key (0-F).
4.  Call `emulator->keyboard.press_key(mapped_key)`.
