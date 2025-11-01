#include "Board.h"
#include <iostream>

int main() {
	std::cout << "Welcome to your C++ Chess Engine!" << std::endl;

	Board myBoard; // Call the constructor and sets up the pieces

	std::cout << "Initial Board:" << std::endl;
	myBoard.print();

	// Make the e2 to e4 pawn move.
	// e2 is (row 6, col 4) --> White piece
	// e4 is row 4, col 4 --> Row 0 is the eighth rank, row 7 first and so on
	Move whitePawnMove = {6, 4, 4, 4}; // {from_row, from_col, to_row, to_col}

	// Make the move
	myBoard.makeMove(whitePawnMove);

	std::cout << "Board after 1. e4:" << std::endl;
	myBoard.print(); // Print the board to see the move

	return 0;
}
