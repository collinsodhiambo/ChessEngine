import pygame
import chess_engine_module
import sys

# CONSTANTS
SCREEN_WIDTH = 512 # 8 Squares * 64 pixels
SCREEN_HEIGHT = 512
SQUARE_SIZE = SCREEN_HEIGHT // 8

# RGB Colors
COLOR_LIGHT = (238, 238, 210) # A light, creamy color
COLOR_DARK = (118, 150, 86)  # A nice green

def draw_board(screen):
	"""
	Draws the 8x8 chessboard pattern.
	"""

	for row in range(8):
		for col in range(8):
			# (row + col) % 2 determines if the square is light or dark
			if (row + col) % 2 == 0:
				color = COLOR_LIGHT
			else:
				color = COLOR_DARK

			# Draw the rectangle
			# (col * SQUARE_SIZE, row * SQUARE_SIZE) is the top-left corner
			pygame.draw.rect(screen, color,
				pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE))


def main():
	""" Main game loop.
	"""

	# Initialization
	pygame.init()
	screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
	pygame.display.set_caption("My C++ Chess Engine")

	# Create our C++ Board
	board = chess_engine_module.Board()

	# Main game loop
	running = True
	while running:
		# Event Handling
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				running = False

		draw_board(screen)

		# Update the Display
		pygame.display.flip()

	# Quit
	pygame.quit()
	sys.exit()

if __name__ == "__main__":
	main()
