import gymnasium as gym
from gymnasium import spaces
import numpy as np
from masterchip8 import masterchip8_py

class Chip8Env(gym.Env):
    """
    Custom Environment that follows gym interface.
    Wraps the C++ MasterChip8 emulator.
    """
    metadata = {'render_modes': ['human', 'rgb_array'], 'render_fps': 60}

    def __init__(self, rom_path, render_mode=None):
        super(Chip8Env, self).__init__()
        
        self.rom_path = rom_path
        self.render_mode = render_mode
        self.cpu = None
        
        # Define action and observation space
        # Chip8 has 16 keys (0-15)
        self.action_space = spaces.Discrete(16)
        
        # Observation is the 64x32 monochrome screen
        # We represent it as a 32x64x1 numpy array (channels last)
        self.observation_space = spaces.Box(low=0, high=1, shape=(32, 64, 1), dtype=np.uint8)
        
        # Initialize the emulator
        self._init_emulator()

    def _init_emulator(self):
        self.cpu = masterchip8_py.SuperChip8(self.rom_path)
        # Run a few ticks to let the game initialize
        for _ in range(50):
            self.cpu.tick()

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        self._init_emulator()
        
        observation = self._get_observation()
        info = {}
        return observation, info

    def step(self, action):
        # Execute action
        # Press key
        self.cpu.press_key(int(action))
        
        # Run for a few frames (frame skip) to let the action take effect
        # Standard Chip8 runs at ~60Hz, so 1-3 frames is usually good for response
        reward = 0
        terminated = False
        truncated = False
        
        # Tick the CPU
        # We might need to tick multiple times per 'step' to speed up training
        # or to match the game speed.
        for _ in range(4): 
            self.cpu.tick()
            
        self.cpu.release_key(int(action))
        
        # Get new state
        observation = self._get_observation()
        
        # TODO: Implement game-specific reward logic and termination conditions
        # This is the hard part: The emulator doesn't inherently know if you won.
        # We will need to inspect memory or screen state to determine rewards.
        
        info = {}
        
        return observation, reward, terminated, truncated, info

    def render(self):
        if self.render_mode == "human":
            # We can use our display_utils here or let the training loop handle it
            pass
        elif self.render_mode == "rgb_array":
            return self._get_observation()

    def _get_observation(self):
        # Extract screen buffer from emulator
        # This is slow if done pixel by pixel in Python. 
        # Ideally, the C++ binding should return a buffer.
        # For now, we iterate.
        screen = np.zeros((32, 64, 1), dtype=np.uint8)
        for y in range(32):
            for x in range(64):
                if self.cpu.get_pixel(x, y):
                    screen[y, x, 0] = 1
        return screen

    def close(self):
        pass
