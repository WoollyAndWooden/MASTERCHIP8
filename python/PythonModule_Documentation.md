# Python Module Documentation

The Python module (`masterchip8_py`) exposes the C++ core to Python using `pybind11`. This allows for headless execution and integration with Machine Learning libraries.

## Architecture

*   **C++ Core**: Compiled as a shared library (`.so` / `.pyd`).
*   **Bindings**: `src/PythonBindings/bindings.cpp` maps C++ classes to Python classes.
*   **Python Scripts**: Import the module to control the emulator.

## Flowcharts

### AI Training Loop
*(Placeholder: Insert Flowchart of RL Training Loop here)*
1.  Initialize Environment (`masterchip8_py.SuperChip8`).
2.  Reset Emulator.
3.  **Loop**:
    *   AI Agent observes state (`get_pixel`).
    *   AI Agent chooses action (`press_key`).
    *   Step Emulator (`tick`).
    *   Calculate Reward.
    *   Update Agent.
