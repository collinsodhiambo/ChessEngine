import pygame
from chess import Board, Move
import sys
import os

# CONSTANTS
SCREEN_WIDTH = 512 # 8 Squares * 64 pixels
SCREEN_HEIGHT = 512
SQUARE_SIZE = SCREEN_HEIGHT // 8

# RGB Colors
COLOR_LIGHT = (238, 238, 210) # A light, creamy color
COLOR_DARK = (118, 150, 86)  # A nice green
HIGHLIGHT_COLOR = (246, 246, 130) # Yellow for selected piece
HIGHLIGHT_LEGAL_COLOR = (186, 202, 68) # Greenish-yellow for legal moves
CHECK_COLOR = (255, 100, 100) # Red color for check

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

def get_row_col_from_mouse(pos):
    """
    Converts a mouse (x, y) position to a (row, col) grid coordinate.
    """
    x, y = pos
    row = y // SQUARE_SIZE
    col = x // SQUARE_SIZE
    return row, col

def draw_game_state(screen, board_state, images, selected_square, legal_moves_for_piece, is_in_check, is_white_turn):
    """
    Draws the entire game state, including the board and the pieces.
    """

    # Draw the 8x8 board state
    draw_board(screen)

    # Highlight for check
    if is_in_check:
        king_to_find = W_KING if is_white_turn else B_KING
        for row in range(8):
            for col in range(8):
                if board_state[row][col] == king_to_find:
                    pygame.draw.rect(screen, CHECK_COLOR,
                        pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE))
                    break # Found the king

    # Highlight the selected square (if any)
    if selected_square:
        row, col = selected_square
        pygame.draw.rect(screen, HIGHLIGHT_COLOR,
            pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE), 5) # Use a border



    # Highlight legal moves (if any)
    for move in legal_moves_for_piece:
        row, col = move.to_row, move.to_col

        # Draw a translucent circle in the center for the square
        center_x = col * SQUARE_SIZE + SQUARE_SIZE // 2
        center_y = row * SQUARE_SIZE + SQUARE_SIZE // 2
        pygame.draw.circle(screen, HIGHLIGHT_COLOR, (center_x, center_y), SQUARE_SIZE // 6)

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

    # Game state variable
    selected_square = None
    legal_moves_for_piece = []

    # Main game loop
    running = True
    while running:
        # Get state from C++
        current_board_state = board.get_board_state()
        is_white_turn = board.is_white_to_move()

        # Ask the C++ engine if the current player is in check
        is_in_check = board.isKingInCheck(is_white_turn)

        # Event Handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            elif event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                row, col = get_row_col_from_mouse(pos)
                clicked_piece = current_board_state[row][col]

                # Check if the click is on a friendly piece
                is_friendly_piece = (is_white_turn and clicked_piece > 0) or \
                                    (not is_white_turn and clicked_piece < 0)

                if selected_square is None:
                    # First Click: Select a piece
                    if is_friendly_piece:
                        selected_square = (row, col)

                        # Get all legal moves, then filter them
                        all_legal_moves = board.getLegalMoves()
                        legal_moves_for_piece = [m for m in all_legal_moves if m.from_row == row and m.from_col == col]

                else:
                    # Second click: Try to make a move
                    from_row, from_col = selected_square
                    to_row, to_col = row, col

                    # Find the move that matches this click
                    move_to_make = None
                    for move in legal_moves_for_piece:
                        if move.to_row == to_row and move.to_col == to_col:
                            # This is a legal move
                            # TODO: Handle promotion piece
                            if move.promotion_piece != EMPTY:
                                # Auto promote to queen
                                if is_white_turn:
                                    move_to_make = Move(from_row, from_col, to_row, to_col, W_QUEEN)
                                else:
                                    move_to_make = Move(from_row, from_col, to_row, to_col, B_QUEEN)
                            else:
                                    move_to_make = move
                            break

                    if move_to_make:
                        board.makeMove(move_to_make)

                    # Reset the click state, whether the move was legal or not
                    selected_square = None
                    legal_moves_for_piece = []


        # Drawing
        draw_game_state(screen, current_board_state, images,
                selected_square, legal_moves_for_piece,
                is_in_check, is_white_turn)

        # Update the Display
        pygame.display.flip()

    # Quit
    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
