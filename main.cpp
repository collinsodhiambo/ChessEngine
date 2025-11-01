#include "Board.h"
#include <iostream>

int main() {
	std::cout << "Welcome to your C++ Chess Engine!" << std::endl;

	Board myBoard; // Call the constructor and sets up the pieces

	myBoard.print();

	return 0;
}
