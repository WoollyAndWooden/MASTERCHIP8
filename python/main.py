import sys
import os

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

try:
    from masterchip8 import masterchip8_py
    print("Successfully imported masterchip8_py!")
    
    try:
        cpu = masterchip8_py.SuperChip8("ROM/pong.ch8")
        print("Emulator initialized.")
        cpu.tick()
        print("Tick successful.")
    except Exception as e:
        print(f"Error running emulator: {e}")
        
except ImportError as e:
    print(f"Failed to import masterchip8_py: {e}")
    print("Did you run the update_lib script?")
