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

	std::cout << "--- Testing En Passant ---" << std::endl;

	// e4
	myBoard.makeMove(Move{6, 4, 4, 4});

	// A nothing move a6
	myBoard.makeMove(Move{1, 0, 2, 0});

	// e5
	myBoard.makeMove(Move{4, 4, 3, 4});

	// f5 (Black pawn moves 2 squares next to white's e5 pawn)
	myBoard.makeMove(Move{1, 5, 3, 5});

	// Board state after Black's f5
	std::cout << "Board state:" << std::endl;
	myBoard.print();


	// -- TEST
	// It's White's turn. The e5 pawn at (3, 4) should have a legal en passant
	// capture to f6 at (2, 5)
	std::cout << "--- Getting White's legal moves... ---" << std::endl;
	std::vector<Move> legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);

	// Find the en passant move
	bool found_en_passant = false;
	Move en_passant_move = Move{3, 4, 2, 5}; // e5 to f6

	for (const Move& move: legalMoves) {
		if (move.from_row == en_passant_move.from_row &&
			move.to_row == en_passant_move.to_row &&
			move.from_col == en_passant_move.from_col &&
			move.to_col == en_passant_move.to_col) {
				found_en_passant = true;
				break;
				}
		}

	if (found_en_passant) {
		std::cout << "\n TEST 1 PASSED: Engine correctly found en passant move!" << std::endl;
	} else {
		std::cout << "\n TEST 1 FAILED: Engine did NOT find en passant move." << std::endl;
	}

	// TEST 2: Make the move
	std::cout << "\n Making En Passant Move..." << std::endl;
	myBoard.makeMove(en_passant_move);
	myBoard.print();
	std::cout << "Check that the White pawn is on f6 (2, 5) and" << std::endl;
	std::cout << "the Black pawn on f5 (3, 5) has been removed." << std::endl;

	return 0;
}
