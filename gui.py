import pygame
import chess
import sys
import os

# --- Constants ---
BOARD_WIDTH = 512  # The 8x8 board
CAPTURE_WIDTH = 120 # NEW: Width of the "graveyard" area
SCREEN_WIDTH = BOARD_WIDTH + CAPTURE_WIDTH # NEW: Total window width
SCREEN_HEIGHT = 512
SQUARE_SIZE = SCREEN_HEIGHT // 8
CAPTURE_IMAGE_SIZE = SQUARE_SIZE // 2 # NEW: Smaller images for captured pieces

# RGB Colors
COLOR_LIGHT = (238, 238, 210)
COLOR_DARK = (118, 150, 86)
HIGHLIGHT_COLOR = (246, 246, 130)
HIGHLIGHT_LEGAL_COLOR = (186, 202, 68)
CHECK_COLOR = (255, 100, 100)
CAPTURE_BG_COLOR = (49, 46, 43)
TEXT_COLOR_LIGHT = (118, 150, 86)
TEXT_COLOR_DARK = (238, 238, 210)

# C++ Enum values
W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING = 1, 2, 3, 4, 5, 6
B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING = -1, -2, -3, -4, -5, -6
EMPTY = 0

def load_images():
    """
    Loads all 12 piece images from the 'images' folder.
    Returns TWO dictionaries:
    1. images (full size)
    2. images_small (half size for captures)
    """
    piece_map = {
        W_PAWN: "wP.svg", W_KNIGHT: "wN.svg", W_BISHOP: "wB.svg",
        W_ROOK: "wR.svg", W_QUEEN: "wQ.svg", W_KING: "wK.svg",
        B_PAWN: "bP.svg", B_KNIGHT: "bN.svg", B_BISHOP: "bB.svg",
        B_ROOK: "bR.svg", B_QUEEN: "bQ.svg", B_KING: "bK.svg"
    }

    images = {}
    images_small = {}

    for piece_val, filename in piece_map.items():
        path = os.path.join("images", filename)
        try:
            image = pygame.image.load(path)
            images[piece_val] = pygame.transform.scale(image, (SQUARE_SIZE, SQUARE_SIZE))
            images_small[piece_val] = pygame.transform.scale(image, (CAPTURE_IMAGE_SIZE, CAPTURE_IMAGE_SIZE))
        except FileNotFoundError:
            print(f"Error: Image file not found at {path}")
            sys.exit()

    return images, images_small

def get_row_col_from_mouse(pos):
    """
    Converts a mouse (x, y) position to a (row, col) grid coordinate.
    UPDATED: Returns None if click is outside the 8x8 board.
    """
    x, y = pos
    if x >= BOARD_WIDTH:
        return None, None # Click was in the capture area

    row = y // SQUARE_SIZE
    col = x // SQUARE_SIZE
    return row, col

def draw_game_state(screen, board_state, images, images_small, selected_square, 
                    legal_moves_for_piece, is_in_check, is_white_turn, 
                    white_captured, black_captured, font):
    """
    Draws the entire game state, including board, pieces, and highlights.
    """
    # 1. Draw the 8x8 chessboard and labels
    for row in range(8):
        for col in range(8):
            is_light = (row + col) % 2 == 0
            color = COLOR_LIGHT if is_light else COLOR_DARK
            pygame.draw.rect(screen, color, 
                             pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE, 
                                         SQUARE_SIZE, SQUARE_SIZE))

            # Draw Rank/File Labels
            text_color = TEXT_COLOR_DARK if is_light else TEXT_COLOR_LIGHT
            if col == 0: # Draw rank number (1-8)
                label = font.render(str(8 - row), True, text_color)
                screen.blit(label, (col * SQUARE_SIZE + 5, row * SQUARE_SIZE + 5))
            if row == 7: # Draw file letter (a-h)
                label = font.render(chr(ord('a') + col), True, text_color)
                rect = label.get_rect(bottomright=(col * SQUARE_SIZE + SQUARE_SIZE - 5, 
                                                   row * SQUARE_SIZE + SQUARE_SIZE - 5))
                screen.blit(label, rect)

    # 2. Highlight for Check
    if is_in_check:
        king_to_find = W_KING if is_white_turn else B_KING
        for row in range(8):
            for col in range(8):
                if board_state[row][col] == king_to_find:
                    pygame.draw.rect(screen, CHECK_COLOR,
                                     pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE,
                                                 SQUARE_SIZE, SQUARE_SIZE))
                    break

    # 3. Highlight the selected square
    if selected_square:
        row, col = selected_square
        pygame.draw.rect(screen, HIGHLIGHT_COLOR,
                         pygame.Rect(col * SQUARE_SIZE, row * SQUARE_SIZE,
                                     SQUARE_SIZE, SQUARE_SIZE), 5)

    # 4. Highlight legal moves
    for move in legal_moves_for_piece:
        row, col = move.to_row, move.to_col
        center_x = col * SQUARE_SIZE + SQUARE_SIZE // 2
        center_y = row * SQUARE_SIZE + SQUARE_SIZE // 2
        pygame.draw.circle(screen, HIGHLIGHT_LEGAL_COLOR, (center_x, center_y), SQUARE_SIZE // 6)

    # 5. Draw the pieces on the board
    for row in range(8):
        for col in range(8):
            piece = board_state[row][col]
            if piece != EMPTY:
                image = images[piece]
                rect = image.get_rect(center = (col * SQUARE_SIZE + SQUARE_SIZE // 2,
                                                row * SQUARE_SIZE + SQUARE_SIZE // 2))
                screen.blit(image, rect)

    # 6. Draw the Captured Pieces Area (NEW)
    pygame.draw.rect(screen, CAPTURE_BG_COLOR,
                     pygame.Rect(BOARD_WIDTH, 0, CAPTURE_WIDTH, SCREEN_HEIGHT))

    x_offset = BOARD_WIDTH + 10
    y_offset_white = 10
    y_offset_black = SCREEN_HEIGHT // 2 + 10

    for piece in black_captured: # White captures Black pieces
        image = images_small[piece]
        screen.blit(image, (x_offset, y_offset_white))
        x_offset += CAPTURE_IMAGE_SIZE
        if x_offset > SCREEN_WIDTH - CAPTURE_IMAGE_SIZE:
            x_offset = BOARD_WIDTH + 10
            y_offset_white += CAPTURE_IMAGE_SIZE

    x_offset = BOARD_WIDTH + 10 # Reset for Black's captures
    for piece in white_captured: # Black captures White pieces
        image = images_small[piece]
        screen.blit(image, (x_offset, y_offset_black))
        x_offset += CAPTURE_IMAGE_SIZE
        if x_offset > SCREEN_WIDTH - CAPTURE_IMAGE_SIZE:
            x_offset = BOARD_WIDTH + 10
            y_offset_black += CAPTURE_IMAGE_SIZE

def draw_game_over_message(screen, message, font):
    # Create a semi-transparent surface
    s = pygame.Surface((BOARD_WIDTH, SCREEN_HEIGHT // 2))
    s.set_alpha(200)  # Set transparency level
    s.fill((40, 40, 40))  # Fill with dark gray color
    screen.blit(s, (0, SCREEN_HEIGHT // 3))  # Draw the surface on the screen

    # Render the message
    text = font.render(message, True, (255, 255, 255))
    rect = text.get_rect(center=(BOARD_WIDTH // 2, SCREEN_HEIGHT // 2 - 20))
    screen.blit(text, rect)

    # Draw the "New Game" Button
    button_text = font.render("New Game", True, (200, 200, 200))
    button_rect = pygame.Rect(BOARD_WIDTH // 2 - 70, SCREEN_HEIGHT // 2 + 10, 140, 40)
    pygame.draw.rect(screen, (80, 80, 80), button_rect)
    text_rect = button_text.get_rect(center = button_rect.center)
    screen.blit(button_text, text_rect)

    return button_rect # Return the rect so we can check for clicks

def main():
    """
    The main game loop.
    """
    pygame.init()
    pygame.font.init()

    # Load a font. Use 'None' for default pygame font. Size 16.
    small_font = pygame.font.SysFont(None, 24)
    large_font = pygame.font.SysFont(None, 48)

    # Adjust screen width
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("My C++ Chess Engine")

    # Load both image dictionaries
    images, images_small = load_images()

    board = chess.Board()

    selected_square = None
    legal_moves_for_piece = []

    # Game state variables
    game_over = False
    game_over_message = ""
    new_game_button_rect = None

    running = True
    while running:
        current_board_state = board.get_board_state()
        is_white_turn = board.is_white_to_move()
        is_in_check = board.isKingInCheck(is_white_turn)

        # Get captured lists from C++
        white_captured = board.getWhiteCaptured()
        black_captured = board.getBlackCaptured()

        # Check game status
        if not game_over:
            status = board.get_game_status()
            if status == chess.GameStatus.WHITE_WINS_CHECKMATE:
                game_over = True
                game_over_message = "Checkmate! White wins."
            elif status == chess.GameStatus.BLACK_WINS_CHECKMATE:
                game_over = True
                game_over_message = "Checkmate! Black wins."
            elif status == chess.GameStatus.DRAW_STALEMATE:
                game_over = True
                game_over_message = "Draw by Stalemate."
            elif status == chess.GameStatus.DRAW_INSUFFICIENT_MATERIAL:
                game_over = True
                game_over_message = "Draw by Insufficient Material."

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            elif event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()

                if game_over:
                    if new_game_button_rect and new_game_button_rect.collidepoint(pos):
                        # Reset the game
                        board = chess.Board()
                        white_captured = []
                        black_captured = []
                        selected_square = None
                        legal_moves_for_piece = []
                        game_over = False
                        game_over_message = ""
                        new_game_button_rect = None
                else:
                    row, col = get_row_col_from_mouse(pos)
                    if row is None: # Click was outside the board
                        continue

                    clicked_piece = current_board_state[row][col]

                    is_friendly_piece = (is_white_turn and clicked_piece > 0) or \
                                    (not is_white_turn and clicked_piece < 0)

                    if selected_square is None:
                        if is_friendly_piece:
                            selected_square = (row, col)
                            all_legal_moves = board.getLegalMoves()
                            legal_moves_for_piece = [m for m in all_legal_moves 
                                                 if m.from_row == row and m.from_col == col]
                    else:
                        from_row, from_col = selected_square
                        to_row, to_col = row, col

                        move_to_make = None
                        for move in legal_moves_for_piece:
                            if move.to_row == to_row and move.to_col == to_col:
                                if move.promotion_piece != EMPTY:
                                    if is_white_turn:
                                        move_to_make = chess.Move(from_row, from_col, to_row, to_col, W_QUEEN)
                                    else:
                                        move_to_make = chess.Move(from_row, from_col, to_row, to_col, B_QUEEN)
                                else:
                                    move_to_make = move
                                break

                        if move_to_make:
                            # Track Captures ---
                            captured_piece = current_board_state[move_to_make.to_row][move_to_make.to_col]
                            moving_piece = current_board_state[move_to_make.from_row][move_to_make.from_col]

                            # 1. Normal Capture
                            if captured_piece != EMPTY:
                                if is_white_turn:
                                    black_captured.append(captured_piece)
                                else:
                                    white_captured.append(captured_piece)

                            # 2. En Passant Capture
                            if (abs(moving_piece) == W_PAWN and
                                captured_piece == EMPTY and
                                move_to_make.from_col != move_to_make.to_col):
                                if is_white_turn:
                                    black_captured.append(B_PAWN)
                                else:
                                    white_captured.append(W_PAWN)

                            board.makeMove(move_to_make) # Make the move

                            # Sort lists to keep them tidy
                            white_captured.sort()
                            black_captured.sort()

                        selected_square = None
                        legal_moves_for_piece = []

        # --- Drawing ---
        # Pass all the new info to our draw function
        draw_game_state(screen, current_board_state, images,
                        images_small,
                        selected_square, legal_moves_for_piece,
                        is_in_check, is_white_turn,
                        white_captured, black_captured,
                        small_font)

        if game_over:
            new_game_button_rect = draw_game_over_message(
                screen, game_over_message, large_font)

        pygame.display.flip()

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
