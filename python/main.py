import sys
import os
import argparse

# Add current directory to path so we can import local modules
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

try:
    from training.tictactoe import train as train_tictactoe
    from training.breakout import train as train_breakout
    from training.invaders import train as train_invaders
    from training.pong import train as train_pong
    import display_utils
except ImportError as e:
    print(f"Failed to import required modules: {e}")
    sys.exit(1)

# Define available games and their ROM paths
ROM_DIR = "TrainingROMS"
GAMES = {
    "tictactoe": {
        "rom": "TICATACTOE.ch8",
        "trainer": train_tictactoe
    },
    "breakout": {
        "rom": "BREAKOUT.ch8",
        "trainer": train_breakout
    },
    "invaders": {
        "rom": "INVADERS.ch8",
        "trainer": train_invaders
    },
    "pong": {
        "rom": "PONG.ch8",
        "trainer": train_pong
    }
}

def main():
    parser = argparse.ArgumentParser(description="MasterChip8 AI Training Runner")
    parser.add_argument("game", choices=GAMES.keys(), help="The game to train on")
    parser.add_argument("--display", action="store_true", help="Enable graphical display of the game")
    
    args = parser.parse_args()
    
    game_config = GAMES[args.game]
    rom_path = os.path.join(ROM_DIR, game_config["rom"])
    
    if not os.path.exists(rom_path):
        print(f"Error: ROM file not found at {rom_path}")
        return

    print(f"Initializing {args.game} with ROM: {rom_path}")
    
    try:
        # The trainer is now the function itself, so we call it directly
        game_config["trainer"](rom_path, args.display)
        
    except Exception as e:
        print(f"An error occurred during execution: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()
