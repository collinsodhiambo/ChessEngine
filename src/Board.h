#ifndef BOARD_H
#define BOARD_H

#include <vector> // To hold a list of moves
#include <utility>

// Use simple integer to represent pieces (for now)
// Positive = White, Negative = Black
// 0 = Empty, 1 = Pawn, 2 = Knight, 3 = Bishop, 4 = Rook, 5 = Queen, 6 = King

enum Piece
{
	EMPTY = 0,
	W_PAWN = 1,
	W_KNIGHT = 2,
	W_BISHOP = 3,
	W_ROOK = 4,
	W_QUEEN = 5,
	W_KING = 6,
	B_PAWN = -1,
	B_KNIGHT = -2,
	B_BISHOP = -3,
	B_ROOK = -4,
	B_QUEEN = -5,
	B_KING = -6
};

enum GameStatus
{
	IN_PROGRESS = 0,
	WHITE_WINS_CHECKMATE = 1,
	BLACK_WINS_CHECKMATE = 2,
	DRAW_STALEMATE = 3,
	DRAW_INSUFFICIENT_MATERIAL = 4
};

// A simple structure to hold move data
struct Move
{
	int from_row;
	int from_col;
	int to_row;
	int to_col;
	int promotion_piece;

	// Constructor for Normal moves
	Move(int fr, int fc, int tr, int tc) : from_row(fr), from_col(fc), to_row(tr), to_col(tc), promotion_piece(EMPTY) {}

	Move(int fr, int fc, int tr, int tc, int pp) : from_row(fr), from_col(fc), to_row(tr), to_col(tc), promotion_piece(pp) {}
};

struct GameState
{
	int board[8][8];
	bool whiteToMove;
	std::pair<int, int> enPassantTarget;
	bool canWhiteKingSide;
	bool canWhiteQueenSide;
	bool canBlackKingSide;
	bool canBlackQueenSide;
	std::vector<int> whiteCaptured;
	std::vector<int> blackCaptured;

	// Add a default constructor
	GameState()
	{
		for (int r = 0; r < 8; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				board[r][c] = EMPTY;
			}
		}
		whiteToMove = true;
		enPassantTarget = {-1, -1};
		canWhiteKingSide = canWhiteQueenSide = canBlackKingSide = canBlackQueenSide = true;
		whiteCaptured.clear();
		blackCaptured.clear();
	}
};

class Board
{
public:
	// Constructor:
	Board();

	// Print the board to the console
	void print();

	// Function to update the board state with a move.
	// We pass the Move object by const reference (&). This avoids making a copy
	// and is more efficient
	void makeMove(const Move &move);

	// A function to get all legal moves for a pawn at a specific square
	// Will return a vector of Move objects
	std::vector<Move> getPawnMoves(int row, int col);

	// Knight's move generator
	std::vector<Move> getKnightMoves(int row, int col);

	// Rook's move generator
	std::vector<Move> getRookMoves(int row, int col);

	// Bishop's move generator
	std::vector<Move> getBishopMoves(int row, int col);

	// Queen's move generator
	std::vector<Move> getQueenMoves(int row, int col);

	// King's move generator
	std::vector<Move> getKingMoves(int row, int col);

	// Master function to get all moves for the current player
	std::vector<Move> getLegalMoves();

	// Checks if a square is attacked by a given side
	bool isSquareAttacked(int row, int col, bool byWhite);

	// Check the current king's status
	// We pass 'true' to check the white king, 'false' for black
	bool isKingInCheck(bool whiteKing);

	// A function to get the board state as a 2D vector
	std::vector<std::vector<int>> getBoardState();

	// Function to say whose turn it is
	bool isWhiteToMove();

	// Function to check game state
	GameStatus getGameStatus();

    bool undoMove();
    bool redoMove();

    std::vector<int> getWhiteCaptured();
    std::vector<int> getBlackCaptured();

private:
	// Helper function to get the character for a piece
	char getPieceChar(int piece);

	// 8x8 array representing the board
	int m_board[8][8];

	// A variable to track whose turn it is
	bool m_whiteToMove;

	// Helper to find the specified king (specified by `bool whiteKing`)
	std::pair<int, int> findKing(bool whiteKing);

	// Store the (row, col) of the square that is vulerable to en passant. (-1, -) if none.
	std::pair<int, int> m_enPassantTarget;

	// Castling rights
	bool m_canWhiteKingSide;
	bool m_canWhiteQueenSide;
	bool m_canBlackKingSide;
	bool m_canBlackQueenSide;

	// Helper for draw detection
	bool checkInsufficientMaterial();

    // Lists for captured pieces
    std::vector<int> m_whiteCaptured;
    std::vector<int> m_blackCaptured;

    // Stacks to hold game history
    std::vector<GameState> m_history;
    std::vector<GameState> m_redoStack;

    // Helper functions to save/load state
    void saveState(GameState& state);
    void restoreState(const GameState& state);
};
#endif // BOARD_H
