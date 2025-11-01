#include "Board.h"
#include <iostream>
#include <vector>

// Helper function to print moves
void printMoves(const std::vector<Move>& moves) {
	if (moves.empty()) {
		std::cout << " (No Moves found)" << std::endl;
		return;
	}

	for (const Move& move : moves) {
		std::cout << " Move from (" <<move.from_row << "," << move.from_col << ")" << " to (" << move.to_row << ","
		<< move.to_col << ")" << std::endl;
	}
}

int main() {
	std::cout << "Welcome to your C++ Chess Engine!" << std::endl;

	Board myBoard; // Call the constructor and sets up the pieces

	std::cout << "Initial Board:" << std::endl;
	myBoard.print();


	// TEST WHITE PAWN
	std::cout << "\nTesting moves for white Pawn at e2 (6, 4):" << std::endl;
	std::vector<Move> e2_pawn_moves = myBoard.getPawnMoves(6, 4);
	printMoves(e2_pawn_moves);

	// TEST BLACK PAWN
	std::cout << "\nTesting moves for black pawns at b7 (1,1):" << std::endl;
	std::vector<Move> b7_pawn_moves = myBoard.getPawnMoves(1,1);
	printMoves(b7_pawn_moves);

	// TEST EMPTY SQUARE
	std::cout << "\nTesting moves for Empty square at e3 (5, 4):" << std::endl;
	std::vector<Move> e3_moves = myBoard.getPawnMoves(5, 4);
	printMoves(e3_moves);

	// Make the e2 to e4 pawn move.
	// e2 is (row 6, col 4) --> White piece
	// e4 is row 4, col 4 --> Row 0 is the eighth rank, row 7 first and so on
	//Move whitePawnMove = {6, 4, 4, 4}; // {from_row, from_col, to_row, to_col}

	// Make the move
	//myBoard.makeMove(whitePawnMove);

	std::cout << "Board after 1. e4:" << std::endl;
	myBoard.print(); // Print the board to see the move

	return 0;
}
