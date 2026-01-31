import gymnasium as gym
from gymnasium import spaces
import numpy as np
from masterchip8 import masterchip8_py
import display_utils
import time

class TicTacToeEnv(gym.Env):
    metadata = {'render_modes': ['human', 'rgb_array'], 'render_fps': 60}

    def __init__(self, rom_path, render_mode=None):
        super(TicTacToeEnv, self).__init__()
        
        self.rom_path = rom_path
        self.render_mode = render_mode
        self.cpu = None
        self.screen = None
        
        if self.render_mode == "human":
            self.screen = display_utils.init_display()
        
        self.action_space = spaces.Discrete(9)
        self.observation_space = spaces.Box(low=0, high=1, shape=(22, 22, 1), dtype=np.uint8)
        self.key_map = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        
        self.p1_score = 0
        self.p2_score = 0
        self.current_player = 1 
        
        self._init_emulator()

    def _init_emulator(self):
        self.cpu = masterchip8_py.SuperChip8(self.rom_path)
        self.cpu.change_cycles_per_tick(30)
        
        for _ in range(50):
            self.cpu.tick()
        self.p1_score = 0
        self.p2_score = 0
        self.current_player = 1

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)
        
        if self.cpu is None:
            self._init_emulator()
        else:
            pass
            
        self.current_player = 1
        return self._get_observation(), {}

    def step(self, action):
        key = self.key_map[action]
        player_name = "P1" if self.current_player == 1 else "P2"
        
        obs_before = self._get_observation()
        pixels_before = np.sum(obs_before)
        
        self.cpu.press_key(key)
        for _ in range(5): self.cpu.tick()
        self.cpu.release_key(key)
        
        for _ in range(20): 
            self.cpu.tick()
            if self.render_mode == "human":
                self.render()
        
        obs_after = self._get_observation()
        pixels_after = np.sum(obs_after)
        
        reward = 0
        terminated = False
        valid_move = False
        
        delta = pixels_after - pixels_before
        
        current_p1 = self.cpu.get_register_V(0xA)
        current_p2 = self.cpu.get_register_V(0xB)
        
        if current_p1 > self.p1_score:
            reward = 10 
            terminated = True
            print(f"P1 WON! Score: {current_p1}-{current_p2}")
            self._handle_game_over_reset()
            
        elif current_p2 > self.p2_score:
            reward = -10 
            terminated = True
            print(f"P2 WON! Score: {current_p1}-{current_p2}")
            self._handle_game_over_reset()
            
        elif delta < -10: 
            reward = 0
            terminated = True
            print("DRAW (Board Cleared)!")
            self._handle_game_over_reset()
            
        elif delta > 0:
            reward = 0.1
            valid_move = True
            print(f"[{player_name}] Move {action} (Key {key}) - VALID (+{delta} px)")
            self.current_player *= -1
            
        else:
            reward = -0.1
            valid_move = False
            print(f"[{player_name}] Move {action} (Key {key}) - INVALID (No Change)")
        
        self.p1_score = current_p1
        self.p2_score = current_p2
        
        return obs_after, reward, terminated, False, {"valid": valid_move}

    def _handle_game_over_reset(self):
        self.cpu.press_key(5)
        for _ in range(5): self.cpu.tick()
        self.cpu.release_key(5)
        
        for _ in range(50):
            self.cpu.tick()
            if self.render_mode == "human": self.render()

    def _get_observation(self):
        crop = np.zeros((22, 22, 1), dtype=np.uint8)
        
        start_x = 21
        start_y = 3
        
        for y in range(22):
            for x in range(22):
                if self.cpu.get_pixel(start_x + x, start_y + y):
                    crop[y, x, 0] = 1
                    
        return crop

    def render(self):
        if self.render_mode == "human" and self.screen:
            display_utils.draw_screen(self.cpu, self.screen)
