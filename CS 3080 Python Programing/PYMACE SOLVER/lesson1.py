import pygame

pygame.init()

WIDTH, HEIGHT = 640, 640
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Lesson 1: the loop")
clock = pygame.time.Clock()

running = True
while running:
    # 1. handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # 2. update state (nothing to update yet)

    # 3. redraw everything from scratch
    screen.fill((30, 30, 30))               # background
    pygame.draw.rect(screen, (200, 60, 60), (270, 270, 100, 100))  # a square
    pygame.display.flip()                    # push the frame to the actual window

    # 4. cap the frame rate
    clock.tick(60)

pygame.quit()