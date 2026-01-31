import pygame

SCALE = 10
WIDTH = 64
HEIGHT = 32

def init_display():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH * SCALE, HEIGHT * SCALE))
    pygame.display.set_caption("MasterChip8")
    return screen

def draw_screen(cpu, screen):
    """
    Draws the 64x32 Chip8 screen to the Pygame window.
    """
    # Handle Pygame events to keep window responsive
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return False
    
    screen.fill((0, 0, 0))
    
    for y in range(HEIGHT):
        for x in range(WIDTH):
            if cpu.get_pixel(x, y):
                pygame.draw.rect(screen, (255, 255, 255), 
                                 (x * SCALE, y * SCALE, SCALE, SCALE))
    
    pygame.display.flip()
    return True
