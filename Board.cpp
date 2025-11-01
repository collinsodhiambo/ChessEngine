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
