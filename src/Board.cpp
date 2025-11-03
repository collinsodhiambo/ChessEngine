#include "Board.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

// Create the constructor for the Board
Board::Board()
{
	m_whiteToMove = true; // White always makes the first move
	m_enPassantTarget = {-1, -1};
	m_canWhiteKingSide = true;
	m_canWhiteQueenSide = true;
	m_canBlackKingSide = true;
	m_canBlackQueenSide = true;

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
		m_board[6][i] = W_PAWN; // White Pawns
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
		for (int col = 0; col < 8; col++)
		{
			m_board[row][col] = EMPTY;
		}
	}

	GameState initial_state;
	saveState(initial_state);
	m_history.push_back(initial_state);
}

// Is it white's turn
bool Board::isWhiteToMove()
{
	return m_whiteToMove;
}

// print function
void Board::print()
{
	std::cout << "\n a b c d e f g h\n";
	std::cout << "-------------------\n";

	for (int row = 0; row < 8; ++row)
	{
		std::cout << 8 - row << "|"; // print row number
		for (int col = 0; col < 8; ++col)
		{
			std::cout << getPieceChar(m_board[row][col]) << "|";
		}
		std::cout << " " << 8 - row << "\n"; // print row number again
	}
	std::cout << "-------------------\n";
	std::cout << " a b c d e f g h\n\n";
}

// Helper function to map piece integers to display characters
char Board::getPieceChar(int piece)
{
	switch (piece)
	{
	case EMPTY:
		return ' ';
	case W_PAWN:
		return 'P';
	case W_KNIGHT:
		return 'N';
	case W_BISHOP:
		return 'B';
	case W_ROOK:
		return 'R';
	case W_QUEEN:
		return 'Q';
	case W_KING:
		return 'K';
	case B_PAWN:
		return 'p';
	case B_KNIGHT:
		return 'n';
	case B_BISHOP:
		return 'b';
	case B_ROOK:
		return 'r';
	case B_QUEEN:
		return 'q';
	case B_KING:
		return 'k';
	default:
		return '?'; // Should not happen
	}
}

void Board::makeMove(const Move &move)
{
	// First get the piece from the 'from' square
	int pieceToMove = m_board[move.from_row][move.from_col];

	// If the King moves, all its castling rights are gone
	if (pieceToMove == W_KING)
	{
		m_canWhiteKingSide = false;
		m_canWhiteQueenSide = false;
	}
	else if (pieceToMove == B_KING)
	{
		m_canBlackKingSide = false;
		m_canBlackQueenSide = false;
	}

	// If a rook moves, its specifi castling right is gone
	if (move.from_row == 7)
	{
		if (move.from_col == 0)
			m_canWhiteQueenSide = false; // a1 rook
		if (move.from_col == 7)
			m_canWhiteKingSide = false; // h1 rook
	}
	// Check for Black rooks
	if (move.from_row == 0)
	{
		if (move.from_col == 0)
			m_canBlackKingSide = false; // a8 rook
		if (move.from_col == 7)
			m_canBlackKingSide = false; // h8 rook
	}

	// Check for an En Passant capture
	// An en passant capture is when a pawn moves diagonally to an EMPTY square
	// but only if that capture is the m_enPassantTarget
	if (std::abs(pieceToMove) == W_PAWN && move.to_row == m_enPassantTarget.first && move.to_col == m_enPassantTarget.second)
	{
		// Remove the enemy pawn
		if (m_whiteToMove)
		{ // white is capturing
			// The black pawn is 1 row below the landing square
			m_board[move.to_row + 1][move.to_col] = EMPTY;
		}
		else
		{
			m_board[move.to_row - 1][move.to_col] = EMPTY;
		}
	}
	// Move it by placing it on the 'to' square
	m_board[move.to_row][move.to_col] = pieceToMove;

	// Clear where the piece move came from
	m_board[move.from_row][move.from_col] = EMPTY;

	// Handle the rook's move if it was a castle
	// Check if the move was a king moving 2 squares
	if (std::abs(pieceToMove) == W_KING && std::abs(move.from_col - move.to_col) == 2)
	{
		// A castle. Move the rook
		if (move.to_col == 6)
		{ // King side
			// Move the rook from h1 to f1
			m_board[move.from_row][5] = m_board[move.from_row][7];
			m_board[move.from_row][7] = EMPTY;
		}
		else if (move.to_col == 2)
		{ // Queenside
			// Move the rook from a1 to d1
			m_board[move.from_row][3] = m_board[move.from_row][0];
			m_board[move.from_row][0] = EMPTY;
		}
	}
	// Handle promotion
	if (move.promotion_piece != EMPTY)
	{
		m_board[move.to_row][move.to_col] = move.promotion_piece;
	}

	// Clear Old target and set new one
	m_enPassantTarget = {-1, -1}; // Clear target from previous turn

	// Check if this move was a 2-square pawn push
	if (std::abs(pieceToMove) == W_PAWN)
	{
		if (std::abs(move.from_row - move.to_row) == 2)
		{
			if (m_whiteToMove)
			{ // White just moved
				m_enPassantTarget = {move.to_row + 1, move.to_col};
			}
			else
			{ // Black just moved
				m_enPassantTarget = {move.to_row - 1, move.to_col};
			}
		}
	}
	// Flip the turn of the player
	m_whiteToMove = !m_whiteToMove;
}

std::vector<Move> Board::getPawnMoves(int row, int col)
{
	std::vector<Move> moves;
	int piece = m_board[row][col];

	if (piece == W_PAWN)
	{
		int one_step_row = row - 1;
		bool is_promotion = (one_step_row == 0);

		// --- One Step Forward ---
		if (one_step_row >= 0 && m_board[one_step_row][col] == EMPTY)
		{
			if (is_promotion)
			{
				moves.push_back(Move{row, col, one_step_row, col, W_QUEEN});
				moves.push_back(Move{row, col, one_step_row, col, W_ROOK});
				moves.push_back(Move{row, col, one_step_row, col, W_BISHOP});
				moves.push_back(Move{row, col, one_step_row, col, W_KNIGHT});
			}
			else
			{
				moves.push_back(Move{row, col, one_step_row, col});
			}
		}

		// --- Two Steps Forward ---
		if (row == 6)
		{
			int two_steps_row = row - 2;
			if (m_board[one_step_row][col] == EMPTY && m_board[two_steps_row][col] == EMPTY)
			{
				moves.push_back(Move{row, col, two_steps_row, col});
			}
		}

		// --- Captures ---
		int d_col[] = {-1, 1}; // {-1: left, 1: right}
		for (int dc : d_col)
		{
			int capture_col = col + dc;
			if (one_step_row >= 0 && capture_col >= 0 && capture_col < 8)
			{
				// Normal Capture
				if (m_board[one_step_row][capture_col] < 0)
				{
					if (is_promotion)
					{
						// THIS IS THE FIX: Use 'capture_col', not 'col'
						moves.push_back(Move{row, col, one_step_row, capture_col, W_QUEEN});
						moves.push_back(Move{row, col, one_step_row, capture_col, W_ROOK});
						moves.push_back(Move{row, col, one_step_row, capture_col, W_BISHOP});
						moves.push_back(Move{row, col, one_step_row, capture_col, W_KNIGHT});
					}
					else
					{
						moves.push_back(Move{row, col, one_step_row, capture_col});
					}
				}
				// En Passant Capture
				else if (one_step_row == m_enPassantTarget.first && capture_col == m_enPassantTarget.second)
				{
					moves.push_back(Move{row, col, one_step_row, capture_col});
				}
			}
		}
	}
	else if (piece == B_PAWN)
	{
		int one_step_row = row + 1;
		bool is_promotion = (one_step_row == 7);

		// --- One Step Forward ---
		if (one_step_row < 8 && m_board[one_step_row][col] == EMPTY)
		{
			if (is_promotion)
			{
				moves.push_back(Move{row, col, one_step_row, col, B_QUEEN});
				moves.push_back(Move{row, col, one_step_row, col, B_ROOK});
				moves.push_back(Move{row, col, one_step_row, col, B_BISHOP});
				moves.push_back(Move{row, col, one_step_row, col, B_KNIGHT});
			}
			else
			{
				moves.push_back(Move{row, col, one_step_row, col});
			}
		}

		// --- Two Steps Forward ---
		if (row == 1)
		{
			int two_steps_row = row + 2;
			if (m_board[one_step_row][col] == EMPTY && m_board[two_steps_row][col] == EMPTY)
			{
				moves.push_back(Move{row, col, two_steps_row, col});
			}
		}

		// --- Captures ---
		int d_col[] = {-1, 1}; // {-1: left, 1: right}
		for (int dc : d_col)
		{
			int capture_col = col + dc;
			if (one_step_row < 8 && capture_col >= 0 && capture_col < 8)
			{
				// Normal Capture
				if (m_board[one_step_row][capture_col] > 0)
				{
					if (is_promotion)
					{
						// THIS IS THE FIX: Use 'capture_col', not 'col'
						moves.push_back(Move{row, col, one_step_row, capture_col, B_QUEEN});
						moves.push_back(Move{row, col, one_step_row, capture_col, B_ROOK});
						moves.push_back(Move{row, col, one_step_row, capture_col, B_BISHOP});
						moves.push_back(Move{row, col, one_step_row, capture_col, B_KNIGHT});
					}
					else
					{
						moves.push_back(Move{row, col, one_step_row, capture_col});
					}
				}
				// En Passant Capture
				else if (one_step_row == m_enPassantTarget.first && capture_col == m_enPassantTarget.second)
				{
					moves.push_back(Move{row, col, one_step_row, capture_col});
				}
			}
		}
	}
	return moves;
}

std::vector<Move> Board::getKnightMoves(int row, int col)
{
	std::vector<Move> moves;

	// Get the color of the knight
	// We can check if the piece > 0 (White) or < 0 (Black)
	bool isWhite = (m_board[row][col] > 0);

	// This array holds all 8 possible "L" shape moves
	// (change in row, change in col)
	int d_row[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int d_col[] = {-1, 1, -2, 2, -2, 2, -1, 1};

	for (int i = 0; i < 8; ++i)
	{
		int new_row = row + d_row[i];
		int new_col = col + d_col[i];

		// Check if the new position is on the board
		if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
		{
			// check the piece at the landing square
			int landing_piece = m_board[new_row][new_col];

			// Is the square empty?
			if (landing_piece == EMPTY)
			{
				moves.push_back(Move{row, col, new_row, new_col});
			}

			// No? Is it an enemy piece?
			else if (landing_piece > 0 && !isWhite)
			{
				moves.push_back(Move{row, col, new_row, new_col});
			}

			else if (landing_piece < 0 && isWhite)
			{
				moves.push_back(Move{row, col, new_row, new_col});
			}
			// If it's a friendly piece, do nothing
		}
	}
	return moves;
}

std::vector<Move> Board::getRookMoves(int row, int col)
{
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col] > 0);

	// Check moves up. Decreasing row
	for (int new_row = row - 1; new_row >= 0; --new_row)
	{
		int landing_piece = m_board[new_row][col];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, new_row, col});
		}
		else if ((landing_piece < 0 && isWhite) || (!isWhite && landing_piece > 0))
		{
			// enemy piece
			moves.push_back(Move{row, col, new_row, col}); // Add capture then stop
			break;
		}
		else
		{
			// It's a friendly piece
			break;
		}
	}

	// check moves down: Increasing row
	for (int new_row = row + 1; new_row < 8; ++new_row)
	{
		int landing_piece = m_board[new_row][col];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, new_row, col});
		}
		else if ((landing_piece < 0 && isWhite) || (!isWhite && landing_piece > 0))
		{
			// enemy piece
			moves.push_back(Move{row, col, new_row, col}); // Add capture then stop
			break;
		}
		else
		{
			// It's a friendly piece
			break;
		}
	}

	// Check moves LEFT. Decreasing col
	for (int new_col = col - 1; new_col >= 0; --new_col)
	{
		int landing_piece = m_board[row][new_col];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, row, new_col});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, row, new_col});
			break;
		}
		else
		{
			break;
		}
	}

	// Check moves RIGHT. Increasing col
	for (int new_col = col + 1; new_col < 8; ++new_col)
	{
		int landing_piece = m_board[row][new_col];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, row, new_col});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, row, new_col});
			break;
		}
		else
		{
			break;
		}
	}
	return moves;
}

std::vector<Move> Board::getBishopMoves(int row, int col)
{
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col] > 0);

	// Check UP-RIGHT (row--, col++)
	for (int r = row - 1, c = col + 1; r >= 0 && c < 8; --r, ++c)
	{
		int landing_piece = m_board[r][c];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, r, c});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, r, c});
			break;
		}
		else
		{
			break;
		}
	}

	// Check Up left (row--, col--)
	for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c)
	{
		int landing_piece = m_board[r][c];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, r, c});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, r, c});
			break;
		}
		else
		{
			break;
		}
	}

	// Check DOWN-RIGHT (row++, col++)
	for (int r = row + 1, c = col + 1; r < 8 && c < 8; ++r, ++c)
	{
		int landing_piece = m_board[r][c];

		if (landing_piece == EMPTY)
		{
			moves.push_back({row, col, r, c});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, r, c});
			break;
		}
		else
		{
			break;
		}
	}
	// Check DOWN-LEFT (row++, col--)
	for (int r = row + 1, c = col - 1; r < 8 && c >= 0; ++r, --c)
	{
		int landing_piece = m_board[r][c];

		if (landing_piece == EMPTY)
		{
			moves.push_back(Move{row, col, r, c});
		}
		else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
		{
			moves.push_back(Move{row, col, r, c});
			break;
		}
		else
		{
			break;
		}
	}

	return moves;
}

std::vector<Move> Board::getQueenMoves(int row, int col)
{
	std::vector<Move> moves = getRookMoves(row, col);
	;
	std::vector<Move> bishopMoves = getBishopMoves(row, col);
	;

	moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

	return moves;
}

std::vector<Move> Board::getKingMoves(int row, int col)
{
	std::vector<Move> moves;
	bool isWhite = (m_board[row][col] > 0);

	// Generate Castling Moves
	if (!isKingInCheck(isWhite))
	{ // Not in check
		if (isWhite)
		{
			// Check White Kingside (0-0)
			if (m_canWhiteKingSide && m_board[7][5] == EMPTY && m_board[7][6] == EMPTY &&
				!isSquareAttacked(7, 5, false) && !isSquareAttacked(7, 6, false))
			{
				moves.push_back(Move{7, 4, 7, 6}); // e1 to g1
			}
			// Check White Queen side (0-0-0)
			if (m_canWhiteQueenSide && m_board[7][1] == EMPTY && m_board[7][2] == EMPTY && m_board[7][3] == EMPTY &&
				!isSquareAttacked(7, 2, false) && !isSquareAttacked(7, 3, false))
			{
				moves.push_back(Move{7, 4, 7, 2}); // e1 to c1
			}
		}
		else
		{
			// Check Black Kingside (0-0)
			if (m_canBlackKingSide && m_board[0][5] == EMPTY && m_board[0][6] == EMPTY &&
				!isSquareAttacked(0, 5, true) && !isSquareAttacked(0, 6, true))
			{
				moves.push_back(Move{0, 4, 0, 6}); // e8 to g8
			}
			// Check Black Queen side (0-0-0)
			if (m_canBlackQueenSide && m_board[0][1] == EMPTY && m_board[0][2] == EMPTY && m_board[0][3] == EMPTY &&
				!isSquareAttacked(0, 2, true) && !isSquareAttacked(0, 3, true))
			{
				moves.push_back(Move{0, 4, 0, 2}); // e8 to c8
			}
		}
	}

	int d_row[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int d_col[] = {-1, 0, 1, -1, 1, -1, 0, 1};

	for (int i = 0; i < 8; ++i)
	{
		int new_row = row + d_row[i];
		int new_col = col + d_col[i];

		// Check if the new position is on the board
		if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8)
		{
			int landing_piece = m_board[new_row][new_col];

			if (landing_piece == EMPTY)
			{
				moves.push_back(Move{row, col, new_row, new_col});
			}
			else if ((isWhite && landing_piece < 0) || (!isWhite && landing_piece > 0))
			{
				moves.push_back(Move{row, col, new_row, new_col});
			}
		}
	}
	return moves;
}

std::vector<Move> Board::getLegalMoves()
{
	std::vector<Move> legalMoves;		// This will hold all the legal moves we find after check-testing
	std::vector<Move> pseudoLegalMoves; // All moves before check-testing

	// Loop over every square on the board
	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 8; ++col)
		{
			int piece = m_board[row][col];
			if (piece == EMPTY)
			{
				continue; // skip empty squares
			}
			// check if the piece belongs to the current player
			// White's turn AND piece is white or vice versa
			if ((m_whiteToMove && piece > 0) || (!m_whiteToMove && piece < 0))
			{
				// Use switch to decide which "specialist" function to call
				// We use std::abs(piece) to treat W_PAWN and B_PAWN the same
				switch (std::abs(piece))
				{
				case W_PAWN:
				{ // Need brackets for variable declaration in a switch
					std::vector<Move> pawnMoves = getPawnMoves(row, col);
					// Add all found moves to our master list
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), pawnMoves.begin(), pawnMoves.end());
				}
				break;

				case W_KNIGHT:
				{
					std::vector<Move> knightMoves = getKnightMoves(row, col);
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), knightMoves.begin(), knightMoves.end());
				}
				break;
				case W_BISHOP:
				{
					std::vector<Move> bishopMoves = getBishopMoves(row, col);
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), bishopMoves.begin(), bishopMoves.end());
				}
				break;
				case W_ROOK:
				{
					std::vector<Move> rookMoves = getRookMoves(row, col);
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), rookMoves.begin(), rookMoves.end());
				}
				break;
				case W_QUEEN:
				{
					std::vector<Move> queenMoves = getQueenMoves(row, col);
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), queenMoves.begin(), queenMoves.end());
				}
				break;
				case W_KING:
				{
					std::vector<Move> kingMoves = getKingMoves(row, col);
					pseudoLegalMoves.insert(pseudoLegalMoves.end(), kingMoves.begin(), kingMoves.end());
				}
				break;
				}
			}
		}
	}

	// Filter for truly legal moves
	for (const Move &move : pseudoLegalMoves)
	{
		// Create a copy of the Board
		Board testBoard = *this;

		// Make the move on the copy
		testBoard.makeMove(move);

		// Check if the King of the player who just moved is in check.
		if (!testBoard.isKingInCheck(m_whiteToMove))
		{
			legalMoves.push_back(move);
		}
	}
	return legalMoves; // Fully filtered
}

bool Board::isSquareAttacked(int row, int col, bool byWhite)
{
	// Check for Pawn attacks
	// We check the squares where an attacking pawn would have to be
	if (byWhite)
	{
		if (row + 1 < 8)
		{
			// Look for a white Pawn at (row + 1, col - 1) or (row + 1, col+1)
			if ((col - 1 >= 0 && m_board[row + 1][col - 1] == W_PAWN) || (col + 1 < 8 && m_board[row + 1][col + 1] == W_PAWN))
			{
				return true;
			}
		}
	}
	else
	{ // Check for Black Pawn
		if (row - 1 >= 0)
		{
			if ((col - 1 >= 0 && m_board[row - 1][col - 1] == B_PAWN) || (col + 1 < 8 && m_board[row - 1][col + 1] == B_PAWN))
			{
				return true;
			}
		}
	}

	// Check for Knigh Attacks
	int d_row[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int d_col[] = {-1, 1, -2, 2, -2, 2, -1, 1};
	int knight_to_find = byWhite ? W_KNIGHT : B_KNIGHT;

	for (int i = 0; i < 8; ++i)
	{
		int r = row + d_row[i];
		int c = col + d_col[i];
		if (r >= 0 && r < 8 && c >= 0 && c < 8 && m_board[r][c] == knight_to_find)
		{
			return true;
		}
	}

	// Check for Sliding attacks (Rook, Bishop, Queen)
	// Check all 8 directions (4 straight, 4 diagonal)

	// Pieces to find
	int rook_to_find = byWhite ? W_ROOK : B_ROOK;
	int bishop_to_find = byWhite ? W_BISHOP : B_BISHOP;
	int queen_to_find = byWhite ? W_QUEEN : B_QUEEN;

	// Straight directions (for Rooks and Queens)
	int straight_dr[] = {-1, 1, 0, 0};
	int straight_dc[] = {0, 0, -1, 1};
	for (int i = 0; i < 4; ++i)
	{
		for (int step = 1;; ++step)
		{
			int r = row + straight_dr[i] * step;
			int c = col + straight_dc[i] * step;
			if (r < 0 || r >= 8 || c < 0 || c >= 8)
				break; // off board

			int piece = m_board[r][c];
			if (piece != EMPTY)
			{
				if (piece == rook_to_find || piece == queen_to_find)
					return true;
				break; // Blocked by another piece
			}
		}
	}

	// Diagonal directions
	int diag_dr[] = {-1, -1, 1, 1};
	int diag_dc[] = {-1, 1, -1, 1};
	for (int i = 0; i < 4; ++i)
	{
		for (int step = 1;; ++step)
		{
			int r = row + diag_dr[i] * step;
			int c = col + diag_dc[i] * step;
			if (r < 0 || r >= 8 || c < 0 || c >= 8)
				break; // Off board

			int piece = m_board[r][c];
			if (piece != EMPTY)
			{
				if (piece == bishop_to_find || piece == queen_to_find)
					return true;
				break; // Blocked by another piece
			}
		}
	}

	// Check for King attacks
	int king_to_find = byWhite ? W_KING : B_KING;
	for (int dr = -1; dr <= 1; ++dr)
	{
		for (int dc = -1; dc <= 1; ++dc)
		{
			if (dr == 0 && dc == 0)
				continue;
			int r = row + dr;
			int c = col + dc;
			if (r >= 0 && r < 8 && c >= 0 && c < 8 && m_board[r][c] == king_to_find)
			{
				return true;
			}
		}
	}

	// If we've checked everything and found nothing
	return false;
}

std::pair<int, int> Board::findKing(bool whiteKing)
{
	int king_to_find = whiteKing ? W_KING : B_KING;

	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 8; ++col)
		{
			if (m_board[row][col] == king_to_find)
			{
				return {row, col};
			}
		}
	}
	// This should never happen in a real game, but it's good to have
	return {-1, -1};
}

bool Board::isKingInCheck(bool whiteKing)
{
	// Find the king
	std::pair<int, int> king_pos = findKing(whiteKing);

	// Check if that square is attacked by the OPPONENT
	// if we're checking the white King, we check for attacks by Black (!whiteKing)
	return isSquareAttacked(king_pos.first, king_pos.second, !whiteKing);
}

std::vector<std::vector<int>> Board::getBoardState()
{
	// Create an 8x8 vector
	std::vector<std::vector<int>> board_state(8, std::vector<int>(8));

	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 8; ++col)
		{
			board_state[row][col] = m_board[row][col];
		}
	}
	return board_state;
}

bool Board::checkInsufficientMaterial()
{
	// This is a helper to check for draws like K vs K, K+N vs K, etc
	int white_knights = 0;
	int black_knights = 0;
	int white_bishops = 0;
	int black_bishops = 0;
	int other_pieces = 0; // Pawns, Rooks, Queens

	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			int piece = m_board[r][c];
			switch (piece)
			{
			case W_KNIGHT:
				white_knights++;
				break;
			case B_KNIGHT:
				black_knights++;
				break;
			case W_BISHOP:
				white_bishops++;
				break;
			case B_BISHOP:
				black_bishops++;
				break;
			case W_PAWN:
			case B_PAWN:
			case W_ROOK:
			case B_ROOK:
			case W_QUEEN:
			case B_QUEEN:
				other_pieces++;
				break;
			}
		}
	}

	// If there are any pawns, rooks, or queens, it's not a draw
	if (other_pieces > 0)
	{
		return false;
	}

	// K vs K
	if (white_knights + white_bishops + black_knights + black_bishops == 0)
	{
		return true;
	}

	// K+N vs K
	if (white_knights == 1 && (black_knights + black_bishops == 0))
	{
		return true;
	}
	if (black_knights == 1 && (white_knights + white_bishops == 0))
	{
		return true;
	}

	// K+B vs K
	if (white_bishops == 1 && (black_knights + black_bishops == 0))
	{
		return true;
	}
	if (black_bishops == 1 && (white_knights + white_bishops == 0))
	{
		return true;
	}

	// K+B vs K+B (both bishops on same color)
	if (white_bishops == 1 && black_bishops == 1 && white_knights == 0 && black_knights == 0)
	{
		// Determine the color of the bishops
		std::pair<int, int> white_bishop_pos = {-1, -1};
		std::pair<int, int> black_bishop_pos = {-1, -1};
		for (int r = 0; r < 8; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				if (m_board[r][c] == W_BISHOP)
				{
					white_bishop_pos = {r, c};
				}
				else if (m_board[r][c] == B_BISHOP)
				{
					black_bishop_pos = {r, c};
				}
			}
		}
		if (white_bishop_pos.first != -1 && black_bishop_pos.first != -1)
		{
			bool white_bishop_color = (white_bishop_pos.first + white_bishop_pos.second) % 2;
			bool black_bishop_color = (black_bishop_pos.first + black_bishop_pos.second) % 2;
			if (white_bishop_color == black_bishop_color)
			{
				return true;
			}
		}
	}
	return false;
}

GameStatus Board::getGameStatus()
{
	// First check for insufficient material
	if (checkInsufficientMaterial())
	{
		return GameStatus::DRAW_INSUFFICIENT_MATERIAL;
	}

	// Get all legal moves
	std::vector<Move> legalMoves = getLegalMoves();

	if (legalMoves.empty())
	{
		// No legal moves available
		if (isKingInCheck(m_whiteToMove))
		{
			return m_whiteToMove ? GameStatus::BLACK_WINS_CHECKMATE : GameStatus::WHITE_WINS_CHECKMATE;
		}
		else
		{
			return GameStatus::DRAW_STALEMATE;
		}
	}
	return GameStatus::IN_PROGRESS;
}

void Board::saveState(GameState& state) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c , 8; ++c) {
            state.board[r][c] = m_board[r][c];
        }
    }

    state.whiteToMove = m_whiteToMove;
    state.enPassantTarget = m_enPassantTarget
