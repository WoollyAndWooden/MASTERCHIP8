import gymnasium as gym
from gymnasium import spaces
import numpy as np
from masterchip8 import masterchip8_py
import display_utils
import time

class PongEnv(gym.Env):
    """
    Custom Environment for Pong (1P) on Chip8.
    Infinite game, score wraps every 10 points.
    """
    metadata = {'render_modes': ['human', 'rgb_array'], 'render_fps': 60}

    def __init__(self, rom_path, render_mode=None):
        super(PongEnv, self).__init__()
        
        self.rom_path = rom_path
        self.render_mode = render_mode
        self.cpu = None
        self.screen = None
        self.clock = None
        
        if self.render_mode == "human":
            self.screen = display_utils.init_display()
            import pygame
            self.clock = pygame.time.Clock()
        
        self.action_space = spaces.Discrete(3)
        self.observation_space = spaces.Box(low=0, high=1, shape=(32, 64, 1), dtype=np.uint8)
        
        self.player_score = 0
        self.opponent_score = 0
        
        self._init_emulator()

    def _init_emulator(self):
        self.cpu = masterchip8_py.SuperChip8(self.rom_path)
        self.cpu.change_cycles_per_tick(10)
        
        for _ in range(50):
            self.cpu.tick()
            
        self.player_score = 0
        self.opponent_score = 0

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        if self.cpu is None:
            self._init_emulator()
        else:
            pass
            
        return self._get_observation(), {}

    def step(self, action):
        key = None
        if action == 1:
            key = 0 
        elif action == 2:
            key = 5 
            
        if key is not None:
            self.cpu.press_key(key)
            
        self.cpu.tick()
            
        if key is not None:
            self.cpu.release_key(key)
            
        if self.render_mode == "human":
            self.render()
        
        obs = self._get_observation()
        
        current_p1 = self.cpu.get_register_V(1)
        current_p2 = self.cpu.get_register_V(2)
        
        reward = 0
        terminated = False
        truncated = False
        
        if current_p1 != self.player_score:
            reward = 1.0 
            print(f"Player Scored! {current_p1}-{current_p2}")
            
        elif current_p2 != self.opponent_score:
            reward = -1.0 
            print(f"Opponent Scored! {current_p1}-{current_p2}")
        else:
            reward = 0
            
        self.player_score = current_p1
        self.opponent_score = current_p2
        
        return obs, reward, terminated, truncated, {}

    def _get_observation(self):
        screen = np.zeros((32, 64, 1), dtype=np.uint8)
        for y in range(32):
            for x in range(64):
                if self.cpu.get_pixel(x, y):
                    screen[y, x, 0] = 1
        return screen

    def render(self):
        if self.render_mode == "human" and self.screen:
            display_utils.draw_screen(self.cpu, self.screen)
            if self.clock:
                self.clock.tick(60)
