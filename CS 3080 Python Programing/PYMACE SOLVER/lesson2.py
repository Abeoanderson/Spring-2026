import pygame

pygame.init()

COLS, ROWS = 10, 10
CELL_SIZE = 50
WIDTH, HEIGHT = COLS * CELL_SIZE, ROWS * CELL_SIZE

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Lesson 2: grid")
clock = pygame.time.Clock()

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
            pygame.draw.rect(screen, (60, 60, 60), rect, width=1)  # just the outline

    pygame.display.flip()
    clock.tick(60)

pygame.quit()