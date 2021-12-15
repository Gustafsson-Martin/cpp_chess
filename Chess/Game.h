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
	NONE,
	WHITE,
	BLACK
};

struct Move {
	int from;
	int to;
};

class Game {
public:
	Game();
	void fen_interpreter(std::string fen);
	std::string fen_generator();
	std::vector<Move> possible_moves();
private:
    static const int BOARD_SIZE{64};
	std::array<Piece, BOARD_SIZE> board{};
	Color turn = Color::WHITE;
	std::vector<int> moves{}; // change from int to Move or something
};

