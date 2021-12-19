#pragma once

#include <array>
#include <vector>
#include <string>

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

enum class Color {
	NONE = 0,
	WHITE = -1,
	BLACK = 1
};

struct Square {
	int row;
	int col;
};

struct Move {
	Square from;
	Square to;
};

struct Vec {
	int x;
	int y;
};

class Game {
public:
	Game();
	void fen_interpreter(const std::string&);
	std::string fen_generator() const;
	std::vector<Move> possible_moves(Square) const;
	void move(Move);
	void print() const;
private:
    static constexpr int BOARD_SIZE{ 8 };
	std::array<std::array<Piece, BOARD_SIZE>, BOARD_SIZE> board{};
	Color turn = Color::WHITE;
	std::vector<int> moves{}; // change from int to Move or something
	int row(int) const;
	int col(int) const;
	bool is_within_board(const Square&) const;
};
