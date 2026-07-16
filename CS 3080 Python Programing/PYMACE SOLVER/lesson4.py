import pygame
import random

pygame.init()

COLS, ROWS = 10, 10
CELL_SIZE = 50
WIDTH, HEIGHT = COLS * CELL_SIZE, ROWS * CELL_SIZE

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Lesson 5: live speed control")
clock = pygame.time.Clock()

COLORS = {
    "empty":    (20, 20, 20),
    "visited":  (40, 70, 120),
    "current":  (200, 60, 60),
}

def fake_walk():
    cells = [(r, c) for r in range(ROWS) for c in range(COLS)]
    random.shuffle(cells)
    for cell in cells:
        yield cell

walker = fake_walk()
cell_states = {}

steps_per_second = 20   # now a live variable, not a constant
frame_count = 0

font = pygame.font.SysFont(None, 24)

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                steps_per_second = min(steps_per_second + 5, 60)
            elif event.key == pygame.K_DOWN:
                steps_per_second = max(steps_per_second - 5, 1)

    frames_per_step = max(60 // steps_per_second, 1)
    frame_count += 1
    if frame_count >= frames_per_step:
        frame_count = 0
        next_cell = next(walker, None)
        if next_cell is not None:
            for k in cell_states:
                if cell_states[k] == "current":
                    cell_states[k] = "visited"
            cell_states[next_cell] = "current"

    screen.fill((20, 20, 20))
    for row in range(ROWS):
        for col in range(COLS):
            x, y = col * CELL_SIZE, row * CELL_SIZE
            rect = (x, y, CELL_SIZE, CELL_SIZE)
            state = cell_states.get((row, col), "empty")
            pygame.draw.rect(screen, COLORS[state], rect)
            pygame.draw.rect(screen, (60, 60, 60), rect, width=1)

    # simple on-screen readout, drawn last so it's on top
    label = font.render(f"speed: {steps_per_second}/sec (UP/DOWN)", True, (255, 255, 255))
    screen.blit(label, (10, HEIGHT - 24))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()