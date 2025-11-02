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

	std::cout << "--- Testing for Fool's Mate ---" << std::endl;

	// f3
	Move m1 = {6, 5, 5, 5}; // (White)
	myBoard.makeMove(m1);

	// e5
	Move m2 = {1, 4, 3, 4}; // (Black)
	myBoard.makeMove(m2);

	// g4
	Move m3 = {6, 6, 4, 6}; // White
	myBoard.makeMove(m3);

	// Qh4# (checkmate)
	Move m4 = {0, 3, 4, 7}; // Black
	myBoard.makeMove(m4);

	// The Board is now checkmate
	myBoard.print();


	// -- TEST
	// It's White's turn, and White is in checkmate
	// getLegalMoves() should return an empty list
	std::cout << "--- Getting White's legal moves... ---" << std::endl;
	std::vector<Move> legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);

	std::cout << "Total moves found: " << legalMoves.size() << std::endl; // Should be 16 + 4 for knight

	if (legalMoves.size() == 0) {
		std::cout << "TEST PASSED: Engine correctly found 0 moves (Checkmate!)" << std::endl;
	}
	else {
		std::cout << "TEST FAILED: Engine found " << legalMoves.size() << " moves." << std::endl;
	}
	return 0;
}
