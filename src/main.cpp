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

	std::cout << "--- Testing Castling ---" << std::endl;

	myBoard.makeMove(Move{6, 4, 4, 4});
        myBoard.makeMove(Move{1, 4, 3, 4});
        myBoard.makeMove(Move{7, 6, 5, 5});
        myBoard.makeMove(Move{0, 1, 2, 2});
        myBoard.makeMove(Move{7, 5, 4, 2});

	std::cout << "Board state:" << std::endl;
	myBoard.print();


	// -- TEST 1: Find White's Castling Move (after Black Moves)
	myBoard.makeMove(Move{0, 5, 3, 2});

	std::cout << "--- Getting White's legal moves... ---" << std::endl;
	std::vector<Move> legalMoves = myBoard.getLegalMoves();
	printMoves(legalMoves);

	// Find the en passant move
	bool found_castle_move = false;
	Move en_passant_move = Move{7, 4, 7, 6}; // e1 to g1

	for (const Move& move: legalMoves) {
		if (move.from_row == en_passant_move.from_row &&
			move.to_row == en_passant_move.to_row &&
			move.from_col == en_passant_move.from_col &&
			move.to_col == en_passant_move.to_col) {
				found_castle_move = true;
				break;
				}
		}

	if (found_castle_move) {
		std::cout << "\n TEST 1 PASSED: Engine correctly found Kingside castling!" << std::endl;
	} else {
		std::cout << "\n TEST 1 FAILED: Engine did NOT find castling move." << std::endl;
	}

	// TEST 2: Make the move
	std::cout << "\n Making Castling Move..." << std::endl;
	myBoard.makeMove(en_passant_move);
	myBoard.print();
	std::cout << "Check that the White King is on g1 (7, 6) and" << std::endl;
	std::cout << "the White rook is on f1 (7, 5) has been removed." << std::endl;

	return 0;
}
