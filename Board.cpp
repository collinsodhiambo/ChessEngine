#include "Board.h"
#include <iostream>
#include <vector>
#include <cmath>

// Create the constructor for the Board
Board::Board() {
	m_whiteToMove = true; // White always makes the first move

	// Set the starting board position
	m_board[0][0] = B_ROOK;
	m_board[0][1] = B_KNIGHT;
	m_board[0][2] = B_BISHOP;
	m_board[0][3] = B_QUEEN;
	m_board[0][4] = B_KING;
        m_board[0][5] = B_BISHOP;
        m_board[0][6] = B_KNIGHT;
        m_board[0][7] = B_ROOK;

	// Place the black and white pawns using a for loop
	for (int i = 0; i < 8; i++)
		{
		m_board[1][i] = B_PAWN; // Black Pawns
		m_board[6][i] = W_PAWN; // White Pawns
		}

	// White pieces
        m_board[7][0] = W_ROOK;
        m_board[7][1] = W_KNIGHT;
        m_board[7][2] = W_BISHOP;
        m_board[7][3] = W_QUEEN;
        m_board[7][4] = W_KING;
        m_board[7][5] = W_BISHOP;
        m_board[7][6] = W_KNIGHT;
        m_board[7][7] = W_ROOK;

	// Initialize all empty squares in the middle
	// Should be all the columns of the rows starting from the third row (black region) to the 7 rows
	for (int row = 2; row < 6; row++)
		{
		for (int col = 0; col < 8; col++){
		m_board[row][col] = EMPTY; }
		}
}

// print function
void Board::print() {
	std::cout << "\n a b c d e f g h\n";
	std::cout << "-------------------\n";

	for (int row = 0; row < 8; ++row){
		std::cout<< 8 - row << "|"; // print row number
		for (int col = 0; col < 8; ++col) {
			std::cout <<getPieceChar(m_board[row][col]) << "|";
			}
		std::cout << " " << 8 - row << "\n"; //print row number again
	}
        std::cout << "-------------------\n";
        std::cout << " a b c d e f g h\n\n";

}

// Helper function to map piece integers to display characters
char Board::getPieceChar(int piece) {
	switch (piece) {
		case EMPTY: return ' ';
		case W_PAWN: return 'P';
                case W_KNIGHT: return 'N';
                case W_BISHOP: return 'B';
                case W_ROOK: return 'R';
                case W_QUEEN: return 'Q';
		case W_KING: return 'K';
		case B_PAWN: return 'p';
                case B_KNIGHT: return 'n';
                case B_BISHOP: return 'b';
                case B_ROOK: return 'r';
                case B_QUEEN: return 'q';
		case B_KING: return 'k';
		default: return '?'; // Should not happen
		}
}

void Board::makeMove(const Move& move) {
	// First get the piece from the 'from' square
	int pieceToMove = m_board[move.from_row][move.from_col];

	// Move it by placing it on the 'to' square
	m_board[move.to_row][move.to_col] = pieceToMove;

	// Clear where the piece move came from
	m_board[move.from_row][move.from_col] = EMPTY;

	// Flip the turn of the player
	m_whiteToMove = !m_whiteToMove;
}

std::vector<Move> Board::getPawnMoves(int row, int col) {
	// Create an empty list (vector) to store the moves we find
	std::vector<Move> moves;

	// Get the piece at a given square
	int piece = m_board[row][col];

	// Logic for white pawns
	if (piece == W_PAWN) {
		// Check one step forward. For white pawns, forward means row index DECREASES
		int one_step_row = row - 1;
		if (one_step_row >= 0 && m_board[one_step_row][col] == EMPTY) {
			// Square is empty and on the board, so its a legal move
			moves.push_back({row, col, one_step_row, col});
		}
		// Check two steps forward (only from the starting rank)
		int two_steps_row = row - 2;
		// Check: (is on starting rank) AND  (one step was clear) AND (two steps is clear)
		if (row == 6 && m_board[one_step_row][col] == EMPTY && m_board[two_steps_row][col] == EMPTY) {
			moves.push_back({row, col, two_steps_row, col});
		}

		// Check captures (left diagonal)
		int capture_left_col = col - 1;
		if (one_step_row >= 0 && capture_left_col >= 0) { // check bounds
			// Check if there is a BLACK piece (piece < 0)
			if (m_board[one_step_row][capture_left_col] < 0) {
				moves.push_back({row, col, one_step_row, capture_left_col});
			}
		}

		// Check captures (right diagonal)
		int capture_right_col = col + 1;
		if (one_step_row >= 0 && capture_right_col < 8) { // Check bounds
			// Check if there is a black piece (piece < 0)
			if (m_board[one_step_row][capture_left_col] < 0) {
				moves.push_back({row, col, one_step_row, capture_left_col});
			}
		}

		// TODO: En Passant
	}

	// BLACK PAWNS
	else if (piece == B_PAWN) {
		// check one step forwad. For black, we increment
		int one_step_row = row + 1;
		if (one_step_row < 8 && m_board[one_step_row][col] == EMPTY) {
			moves.push_back({row, col, one_step_row, col});
		}

		// Check two steps forward (only from starting rank)
		int two_steps_row = row + 2;
		if (row == 1 && m_board[one_step_row][col] == EMPTY && m_board[two_steps_row][col] == EMPTY) {
			moves.push_back({row, col, two_steps_row, col});
		}

		// Check captures (left diagonal)
		int capture_left_col = col - 1;
		if (one_step_row < 8 && capture_left_col >= 0) { // Check bounds
			// check if there is a WHITE piece (piece > 0)
			if (m_board[one_step_row][capture_left_col] > 0) {
				moves.push_back({row, col, one_step_row, capture_left_col});
			}
		}

		// Check captures (left right diagonal)
		int capture_right_col = col + 1;
		if (one_step_row < 8 && capture_left_col < 8) {
			// Check if there is a white piece (piece > 0)
			if (m_board[one_step_row][capture_right_col] > 0) {
				moves.push_back({row, col, one_step_row, capture_right_col});
			}
		}
	}
	// Return the list of moves we found
	return moves;
}

std::vector<Move> Board::getKnightMoves(int row, int col) {
	std::vector<Move> moves;

	// Get the color of the knight
	// We can check if the piece > 0 (White) or < 0 (Black)
	bool isWhite = (m_board[row][col] > 0);

	// This array holds all 8 possible "L" shape moves
	// (change in row, change in col)
	int d_row[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int d_col[] = {-1, 1, -1, 2, -2, 2, -1, 1};

	for (int i = 0; i < 8; ++i) {
		int new_row = row + d_row[i];
		int new_col = col + d_col[i];

		// Check if the new position is on the board
		if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8) {
			// check the piece at the landing square
			int landing_piece = m_board[new_row][new_col];

			// Is the square empty?
			if (landing_piece == EMPTY) {
				moves.push_back({row, col, new_row, new_col});
			}

			// No? Is it an enemy piece?
			else if (landing_piece > 0 && !isWhite) {
				moves.push_back({row, col, new_row, new_col});
			}

			else if (landing_piece < 0 && isWhite) {
				moves.push_back({row, col, new_row, new_col});
			}
			// If it's a friendly piece, do nothing
		}
	}
	return moves;
}


std::vector<Move> Board::getRookMoves(int row, int col) {
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col] > 0);

	// Check moves up. Decreasing row
	for (int new_row = row - 1; new_row >= 0; --new_row) {
		int landing_piece = m_board[new_row][col];

		if (landing_piece == EMPTY) {
			moves.push_back({row, col, new_row, col});
		}
		else if ((landing_piece < 0 && isWhite) || (!isWhite && landing_piece > 0)) {
			// enemy piece
			moves.push_back({row, col, new_row, col}); // Add capture then stop
			break;
		}
		else {
			// It's a friendly piece
			break;
		}
	}

	// check moves down: Increasing row
	for (int new_row = row + 1; row < 8; ++new_row) {
		int landing_piece = m_board[new_row][col];

                if (landing_piece == EMPTY) {
                        moves.push_back({row, col, new_row, col});
                }
                else if ((landing_piece < 0 && isWhite) || (!isWhite && landing_piece > 0)) {
                        // enemy piece
                        moves.push_back({row, col, new_row, col}); // Add capture then stop
                        break;
                }
                else {
                        // It's a friendly piece
                        break;
                }
        }


	// Check moves LEFT. Decreasing col
	for (int new_col = col - 1; new_col >= 0; --new_col) {
		int landing_piece = m_board[row][new_col];

		if (landing_piece == EMPTY) {
			moves.push_back({row, col, row, new_col});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
			moves.push_back({row, col, row, new_col});
			break;
		}
		else {
			break;
		}
	}

	// Check moves RIGHT. Increasing col
        for (int new_col = col + 1; new_col < 8; ++new_col) {
                int landing_piece = m_board[row][new_col];

                if (landing_piece == EMPTY) {
                        moves.push_back({row, col, row, new_col});
                }
                else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
                        moves.push_back({row, col, row, new_col});
                        break;
                }
                else {
                        break;
                }
        }
	return moves;
}

std::vector<Move> Board::getBishopMoves(int row, int col) {
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col] > 0);

	// Check UP-RIGHT (row--, col++)
	for (int r = row - 1, c = col + 1; r >= 0 && c < 8; --r, ++c) {
		int landing_piece = m_board[r][c];

		if (landing_piece == EMPTY) {
			moves.push_back({row, col, r, c});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
			moves.push_back({row, col, r, c});
			break;
		}
		else {
			break;
		}
	}

	// Check Up left (row--, col--)
        for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
                int landing_piece = m_board[r][c];

                if (landing_piece == EMPTY) {
                        moves.push_back({row, col, r, c});
                }
                else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
                        moves.push_back({row, col, r, c});
                        break;
                }
                else {
                        break;
                }
	}

	// Check DOWN-RIGHT (row++, col++)
        for (int r = row + 1, c = col + 1; r < 8 && c < 8; ++r, ++c) {
                int landing_piece = m_board[r][c];

                if (landing_piece == EMPTY) {
                        moves.push_back({row, col, r, c});
                }
                else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
                        moves.push_back({row, col, r, c});
                        break;
                }
                else {
                        break;
                }

	}
	// Check DOWN-LEFT (row++, col--)
        for (int r = row + 1, c = col - 1; r < 8 && c >= 0; ++r, --c) {
                int landing_piece = m_board[r][c];

                if (landing_piece == EMPTY) {
                        moves.push_back({row, col, r, c});
                }
                else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
                        moves.push_back({row, col, r, c});
                        break;
                }
                else {
                        break;
                }
	}

	return moves;
}


std::vector<Move> Board::getQueenMoves(int row, int col) {
	std::vector<Move> moves = getRookMoves(row, col);;
	std::vector<Move> bishopMoves = getBishopMoves(row, col);;

	moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

	return moves;
}

std::vector<Move> Board::getKingMoves(int row, int col) {
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col]);

	int d_row[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int d_col[] = {-1, 0, 1, -1, 1, -1, 0, 1};

	for (int i = 0; i < 8; ++i) {
		int new_row = row + d_row[i];
		int new_col = col + d_col[i];

		// Check if the new position is on the board
		if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8) {
			int landing_piece = m_board[new_row][new_col];

			if (landing_piece == EMPTY) {
				moves.push_back({row, col, new_row, new_col});
			}
			else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0)) {
				moves.push_back({row, col, new_row, new_col});
			}
		}
	}
	return moves;
}

std::vector<Move> Board::getLegalMoves() {
	std::vector<Move> allMoves; // This will hold all the moves we find

	// Loop over every square on the board
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			int piece = m_board[row][col];
			if (piece == EMPTY) {
				continue; // skip empty squares
			}
			// check if the piece belongs to the current player
			// White's turn AND piece is white or vice versa
			if ((m_whiteToMove && piece > 0) || (!m_whiteToMove && piece < 0)) {
				// Use switch to decide which "specialist" function to call
				// We use std::abs(piece) to treat W_PAWN and B_PAWN the same
				switch (std::abs(piece)) {
					case W_PAWN:
						{	// Need brackets for variable declaration in a switch
							std::vector<Move> pawnMoves = getPawnMoves(row, col);
							// Add all found moves to our master list
							allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
						}
						break;

					case W_KNIGHT:
						{
							std::vector<Move> knightMoves = getKnightMoves(row, col);
							allMoves.insert(allMoves.end(), knightMoves.begin(), knightMoves.end());
						}
						break;
                                        case W_BISHOP:
						{
                                                 	std::vector<Move> bishopMoves = getBishopMoves(row, col);
						        allMoves.insert(allMoves.end(), bishopMoves.begin(), bishopMoves.end());
                                                }
					       break;
                                        case W_ROOK:
						{
							std::vector<Move> rookMoves = getRookMoves(row, col);
                                                        allMoves.insert(allMoves.end(), rookMoves.begin(), rookMoves.end());
						}
                                                break;
                                        case W_QUEEN:
						{
							std::vector<Move> queenMoves = getQueenMoves(row, col);
                                                    allMoves.insert(allMoves.end(), queenMoves.begin(), queenMoves.end());
                                                }
						 break;
                                        case W_KING:
						{
							std::vector<Move> kingMoves = getKingMoves(row, col);
                                                        allMoves.insert(allMoves.end(), kingMoves.begin(), kingMoves.end());
						}
						break;

					}
				}
			}
		}
	return allMoves;
}

bool Board::isSquareAttacked(int row, int col, bool byWhite) {
	// Check for Pawn attacks
	// We check the squares where an attacking pawn would have to be
	if (byWhite) {
		// Look for a white Pawn at (row + 1, col - 1) or (row + 1, col+1)
		if ((col - 1 >= 0 && m_board[row + 1][col - 1] == W_PAWN) || (col + 1 < 8 && m_board[row + 1][col + 1] == W_PAWN)) {
			return true;
		}
	}
	else { // Check for Black Pawn
		if ((col - 1 >= 0 &&m_board[row - 1][col - 1] == B_PAWN) || (col + 1 < 8 && m_board[row-1][col + 1] == B_PAWN)) {
			return true;
		}
	}

	// Check for Knigh Attacks
	int d_row[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int d_col[] = {-1, 1, -2, 2, -2, 2, -1, 1};
	int knight_to_find = byWhite ? W_KNIGHT: B_KNIGHT;

	for (int i = 0; i < 8; ++i) {
		int r = row + d_row[i];
		int c = col + d_col[i];
		if (r >= 0 && r < 8 && c >= 0 && c < 8 && m_board[r][c] == knight_to_find) {
			return true;
		}
	}

	// Check for Sliding attacks (Rook, Bishop, Queen)
	// Check all 8 directions (4 straight, 4 diagonal)

	// Pieces to find
	int rook_to_find = byWhite ? W_ROOK : B_ROOK;
	int bishop_to_find = byWhite ? W_BISHOP : B_BISHOP;
	int queen_to_find = byWhite ? W_QUEEN : B_QUEEN;

	// Straight directions (for Rooks and Queens)
	int straight_dr[] = {-1, 1, 0 , 0};
	int straight_dc[] = {0, 0, -1, 1};
	for (int i = 0; i < 4; ++i) {
		for (int step = 1; ; ++step) {
			int r = row + straight_dr[i] * step;
			int c = col + straight_dc[i] * step;
			if (r < 0 || r >= 8 || c < 0 || c >= 8) break; //off board

			int piece = m_board[r][c];
			if (piece != EMPTY) {
				if (piece == rook_to_find || piece == queen_to_find) return true;
				break; // Blocked by another piece
			}
		}
	}

	// Diagonal directions
	int diag_dr[] = {-1, -1, 1, 1};
	int diag_dc[] = {-1, 1, -1, 1};
	for (int i = 0; i < 4; ++i) {
		for (int step = 1; ; ++step) {
			int r = row + diag_dr[i] * step;
			int c = row + diag_dc[i] * step;
			if (r < 0 || r >= 8 || c < 0 || c >= 8) break; // Off board

			int piece = m_board[r][c];
			if (piece != EMPTY) {
				if (piece == bishop_to_find || piece == queen_to_find) return true;
				break; // Blocked by another piece
			}
		}
	}

	// Check for King attacks
	int king_to_find = byWhite ? W_KING : B_KING;
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0) continue;
			int r = row + dr;
			int c = col + dc;
			if (r >= 0 && r < 8 && c >= 0 && c < 8 && m_board[r][c] == king_to_find) {
				return true;
			}
		}
	}

	// If we've checked everything and found nothing
	return false;
}



