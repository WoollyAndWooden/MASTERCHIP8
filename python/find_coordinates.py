import sys
import os
import time

# Add current directory to path
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from masterchip8 import masterchip8_py

def scan_screen(cpu):
    on_pixels = set()
    for y in range(32):
        for x in range(64):
            if cpu.get_pixel(x, y):
                on_pixels.add((x, y))
    return on_pixels

def main():
    rom_path = "TrainingROMS/TICATACTOE.ch8"
    cpu = masterchip8_py.SuperChip8(rom_path)
    
    # Wait for game to load
    for _ in range(100): cpu.tick()
    
    base_pixels = scan_screen(cpu)
    print(f"Base pixels: {len(base_pixels)}")
    
    # Try keys 0, 1, 2, 4 (Top Left, Top Mid, Top Right, Mid Left)
    keys_to_test = [0, 1, 2, 4]
    
    for i, key in enumerate(keys_to_test):
        player = "P1" if i % 2 == 0 else "P2"
        print(f"\n--- Pressing Key {key} ({player}) ---")
        
        cpu.press_key(key)
        for _ in range(10): cpu.tick()
        cpu.release_key(key)
        
        # Wait for move to process
        for _ in range(50): cpu.tick()
        
        current_pixels = scan_screen(cpu)
        new_pixels = current_pixels - base_pixels
        
        if not new_pixels:
            print("No new pixels detected!")
        else:
            # Find bounding box of new pixels
            xs = [p[0] for p in new_pixels]
            ys = [p[1] for p in new_pixels]
            min_x, max_x = min(xs), max(xs)
            min_y, max_y = min(ys), max(ys)
            
            print(f"Detected Shape at: X[{min_x}-{max_x}], Y[{min_y}-{max_y}]")
            print(f"Pixels: {list(new_pixels)}")
            
        # Update base for next move
        base_pixels = current_pixels

if __name__ == "__main__":
    main()
