import pygame
from chess import Board
import sys
import os

# CONSTANTS
SCREEN_WIDTH = 512 # 8 Squares * 64 pixels
SCREEN_HEIGHT = 512
SQUARE_SIZE = SCREEN_HEIGHT // 8

# RGB Colors
COLOR_LIGHT = (238, 238, 210) # A light, creamy color
COLOR_DARK = (118, 150, 86)  # A nice green

# C++ Enum values
W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING = list(range(1,7))
B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING = list(range(-1,-7, -1))
EMPTY = 0

def load_images():
    """
    Loads all 12 pieces from the 'images' folder.
    Returns a dictionary mapping the piece's C++ enum value to its scaled Pygame image.
    """
    piece_map = {
        W_PAWN: "wP.svg",
        W_KNIGHT: "wN.svg",
        W_BISHOP: "wB.svg",
        W_ROOK: "wR.svg",
        W_QUEEN: "wQ.svg",
        W_KING: "wK.svg",
        B_PAWN: "bP.svg",
        B_KNIGHT: "bN.svg",
        B_BISHOP: "bB.svg",
        B_ROOK: "bR.svg",
        B_QUEEN: "bQ.svg",
        B_KING: "bK.svg",
    }

    images = {}
    for piece_val, filename in piece_map.items():
        path = os.path.join('images', filename)
        try:
            image = pygame.image.load(path)

            # Scale the image to fit the square dimensions
            images[piece_val] = pygame.transform.scale(image, (SQUARE_SIZE, SQUARE_SIZE))

        except FileNotFoundError:
            print(f"Error: Image file not found at {path}")
            print("12 files are required and should be named as wP, wB for white pawn and Bishop and bP, bB for blacks")
            sys.exit()
    return images


def draw_game_state(screen, board_state, images):
    """
    Draws the entire game state, including the board and the pieces.
    """

    # Draw the 8x8 board state
    draw_board(screen)

    # Draw the pieces
    for row in range(8):
        for col in range(8):
            piece = board_state[row][col]
            if piece != EMPTY:
                # Get the correct image from our dictionary
                image = images[piece]

                # Get the rectangle for the image and center it on the square
                rect = image.get_rect(center = (col * SQUARE_SIZE + SQUARE_SIZE // 2, row * SQUARE_SIZE + SQUARE_SIZE // 2))

                # Draw the image
                screen.blit(image, rect)

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

    # Load assets
    images = load_images()

    # Create our C++ Board
    board = Board()

    # Main game loop
    running = True
    while running:
        # Event Handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

		# Get state from C++
        # We get 8x8 list from C++ every frame
        current_board_state = board.get_board_state()

        # Drawing
        draw_game_state(screen, current_board_state, images)

        # Update the Display
        pygame.display.flip()

    # Quit
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
