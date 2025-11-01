#include "Board.h"
#include <iostream>
#include <vector>
#include <iomanip>

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


	// We'll use std::boolalpha to print "true/false"
	std::cout << std::boolalpha;

	// Test some squares
	std::cout << "--- Testing Attack Sensor --- " << std::endl;

	// e3 (row 5, col 4): Should be attacked by White d2-pawn and f2-pawn
	std::cout << "Is e3 (5, 4) attacked by White? "
		  << myBoard.isSquareAttacked(5, 4, true) << std::endl; // Should be true

	// e3 (row 5, col 4): Should NOT be attacked by Black
	std::cout << "Is e3 (5, 4) attacked by Black? "
		  << myBoard.isSquareAttacked(5, 4, false) << std::endl; // Should be false

	// d3 (row 5, col 3): Attacked by White (c2-pawn, e2-pawn, d1-queen)
	std::cout << " Is d3 (5, 3) attacked by White? "
		  << myBoard.isSquareAttacked(5, 3, true) << std::endl; // Should be true

	// f6 (row 2, col 5): Attacked by Black (e7-pawn, g7-pawn, g8-knight)
	std::cout << "Is f6 (2, 5) attacked by Black? "
		  << myBoard.isSquareAttacked(2, 5, false) << std::endl; // Should be true

	// TEST WHITE PAWN
	std::cout << "\nGetting White's initial moves..." << std::endl;
	std::vector<Move> legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);
	std::cout << "Total moves found: " << legalMoves.size() << std::endl; // Should be 16 + 4 for knight

	// Make a move (e2 to e4)
	std::cout << "\n --- Making a move e2 to e4 ---\n" <<std::endl;
	Move e4_move = {6, 4, 4, 4};
	myBoard.makeMove(e4_move);
	myBoard.print();


	// TEST BLACK PAWN
	std::cout << "\nGetting Black's moves...:" << std::endl;
	legalMoves = myBoard.getLegalMoves();
	std::cout << "Total moves found: " << legalMoves.size() << std::endl; // 16 + 4
	myBoard.print(); // Print the board to see the move

	return 0;
}
