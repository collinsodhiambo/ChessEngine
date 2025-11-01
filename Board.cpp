#include "Board.h"
#include <iostream>

// Create the constructor for the Board
Board::Board() {
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
		m_board[6][i] = W_PAWN; // White Pawsn
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
void Board::print(){
	std::cout << "\n a b c d e f g h\n";
	std::count << "-------------------\n";

	for (int row = 0; row < 8; ++row){
		std::count<< 8 - row << "|"; // print row number
		for (int col = 0; col < 8; ++col) {
			std::count <<getPieceChar(m_board[row][col]) << "|";
			}
		std::cout << " " << 8 - row << "\n"; //print row number again
        std::count << "-------------------\n";
        std::cout << " a b c d e f g h\n\n";

