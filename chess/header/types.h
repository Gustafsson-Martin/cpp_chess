#pragma once

#include <array>

enum class Piece : char {
	NONE = '\0',
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

using BoardType = std::array<std::array<Piece, 8>, 8>;