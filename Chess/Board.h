#pragma once

#include <array>
#include <vector>
#include <string>

struct Square {
	int col;
	int row;
};

struct Move {
	Square from;
	Square to;
};

enum class Piece : char {
	EMPTY = '\0',
	WHITE_PAWN = 'P',
	WHITE_ROOK = 'R',
	WHITE_KNIGHT = 'N',
	WHITE_BISHOP = 'B',
	WHITE_QUEEN = 'Q',
	WHITE_KING = 'K',
	BLACK_PAWN = 'p',
	BLACK_ROOK = 'r',
	BLACK_KNIGHT = 'n',
	BLACK_BISHOP = 'b',
	BLACK_QUEEN = 'q',
	BLACK_KING = 'k'
};

enum class PieceType : char {
	EMPTY = '\0',
	PAWN = 'p',
	ROOK = 'r',
	KNIGHT = 'n',
	BISHOP = 'b',
	QUEEN = 'q',
	KING = 'k'
};

enum class Color {
	NONE = 0,
	WHITE = -1,
	BLACK = 1
};

struct Vec {
	int x;
	int y;
};

class Board {
public:
	Board();
	void print() const;
	void print_possible_moves(std::vector<Move>) const;
	Piece& operator[](const Square&);
	Piece at(const Square& square) const;
	std::array<Piece, 8>& operator[](const int);
	void fen_interpreter(const std::string&);
	std::string fen_generator() const;
	std::vector<Move> possible_moves(const Square&);
	void move(Move);
private:
	static constexpr int BOARD_LENGTH{8};
	std::array<std::array<Piece, BOARD_LENGTH>, BOARD_LENGTH> board{};
	bool is_within_board(const Square& square) const;
	bool is_legal_move(const Move& move) const;
	bool is_taking_move(const Move& move) const;
};