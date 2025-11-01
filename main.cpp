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
	std::cout << "\nGetting White's initial moves..." << std::endl;
	std::vector<Move> legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);
	std::cout << "Total moves found: " << legalMoves.size() << std::endl; // Should be 16

	// Make a move (e2 to e4)
	std::cout << "\n --- Making a move e2 to e4 ---\n" <<std::endl;
	Move e4_move = {6, 4, 4, 4};
	myBoard.makeMove(e4_move);
	myBoard.print();


	// TEST BLACK PAWN
	std::cout << "\nGetting Black's moves...:" << std::endl;
	legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);
	std::cout << "Total moves found: " << legalMoves.size() << std::endl; // 16
	myBoard.print(); // Print the board to see the move

	return 0;
}
