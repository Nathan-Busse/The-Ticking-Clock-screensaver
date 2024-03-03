import pygame
import time

# Set the display size
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

# Initialize Pygame
pygame.init()
pygame.font.init()

# Set up the screen
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Bomb Countdown")

# Define colors
RED = (255, 0, 0)
BLACK = (0, 0, 0)

# Set up the font
font = pygame.font.Font(None, 100)

# Countdown timer function
def countdown(minutes):
    start_time = time.time()
    ticking_sound = pygame.mixer.Sound("tick.wav")
    ticking_sound.set_volume(0.5)
    ticking_sound.play(-1)  # Looping the ticking sound
    while True:
        remaining_seconds = int((minutes * 60) - (time.time() - start_time))
        if remaining_seconds <= 0:
            break
        screen.fill(BLACK)
        remaining_minutes = remaining_seconds // 60
        text = font.render("Time Left: {:02d}:{:02d}".format(remaining_minutes, remaining_seconds % 60), True, RED)
        text_rect = text.get_rect(center=(SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2))
        screen.blit(text, text_rect)
        pygame.display.flip()
        pygame.time.delay(1000)  # Update every second
    ticking_sound.stop()  # Stop the ticking sound when countdown finishes

# Main function
def main():
    countdown(5)  # 5-minute countdown

if __name__ == "__main__":
    main()
