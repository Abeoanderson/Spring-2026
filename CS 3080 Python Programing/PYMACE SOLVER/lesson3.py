import pygame

pygame.init()

COLS, ROWS = 10, 10
CELL_SIZE = 50
WIDTH, HEIGHT = COLS * CELL_SIZE, ROWS * CELL_SIZE

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Lesson 3: cell state")
clock = pygame.time.Clock()

# --- this dict is standing in for your future "step" object ---
# keys are (row, col), values are a state label
cell_states = {
    (2, 2): "current",
    (2, 3): "frontier",
    (3, 2): "frontier",
    (1, 2): "visited",
    (0, 2): "visited",
}

COLORS = {
    "empty":    (20, 20, 20),
    "visited":  (40, 70, 120),
    "frontier": (200, 170, 60),
    "current":  (200, 60, 60),
}

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill((20, 20, 20))

    for row in range(ROWS):
        for col in range(COLS):
            x = col * CELL_SIZE
            y = row * CELL_SIZE
            rect = (x, y, CELL_SIZE, CELL_SIZE)

            state = cell_states.get((row, col), "empty")
            color = COLORS[state]

            pygame.draw.rect(screen, color, rect)              # fill
            pygame.draw.rect(screen, (60, 60, 60), rect, width=1)  # outline on top

    pygame.display.flip()
    clock.tick(60)

pygame.quit()