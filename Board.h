#ifndef BOARD_H
#define BOARD_H

// Use simple integer to represent pieces (for now)
// Positive = White, Negative = Black
// 0 = Empty, 1 = Pawn, 2 = Knight, 3 = Bishop, 4 = Rook, 5 = Queen, 6 = King

enum Piece { EMPTY = 0, W_PWAN = 1, W_KNIGHT = 2, W_BISHOP = 3, W_ROOK = 4, W_QUEEN = 5, W_KING = 6,
	B_PAWN = -1, B_KNIGHT = -2, B_BISHOP = -3, B_ROOK = -4, B_QUEEN = -5, B_KING = -6 };

class Board {
	public:
		// Constructor:
		Board();

		// Print the board to the console
		void print();

		// 8x8 array representing the board
		int m_board[8][8];

		// Helper function to get the character for a piece
		char getPiece(int piece);
	};
#endif //BOARD_H
