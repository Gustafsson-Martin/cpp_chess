#include "Game.h"
#include <iostream>

Game::Game() {
}

int Game::row(int index) const {
	return index / 8;
}

int Game::col(int index) const {
	return index % 8;
}


Color piece_color(Piece piece) {
	if (piece == Piece::EMPTY) return Color::NONE;
	if ((char)piece > 'a') return Color::BLACK;
	return Color::WHITE;
}

void Game::fen_interpreter(const std::string& fen) {
	// TODO: Do more than just set the board (set which sides turn it is etc.)
	int board_index = 0;
	for (const char &chr : fen) {
		if (chr == ' ') break;
		if (chr == '/') continue;
		if (isdigit(chr)) {
			board_index += static_cast<int>(chr) - '0';
		} else {
			board[board_index / BOARD_SIZE][board_index % BOARD_SIZE] = static_cast<Piece>(chr);
			board_index++;
		}
	}
}

std::string Game::fen_generator() const {
	std::string fen{};
	int empty_counter{};
	for (const std::array<Piece, BOARD_SIZE>& row : board) {
		for (Piece piece : row) {
			if (piece == Piece::EMPTY) empty_counter++;
			else {
				if (empty_counter > 0) fen += std::to_string(empty_counter);
				empty_counter = 0;
				fen += static_cast<char>(piece);
			}
		}
		if (empty_counter > 0) fen += std::to_string(empty_counter);
		empty_counter = 0;
		fen += "/";
	}
	fen.pop_back(); // Remove trailing "/"
	fen += " w - - 0 1";
	return fen;
}

bool Game::is_within_board(const Square& square) const {
	return (square.row >= 0 && square.row < BOARD_SIZE&& square.col >= 0 && square.col < BOARD_SIZE);
}

Square add(const Square& square, const Vec& vec) {
	return Square{square.row + vec.y, square.col + vec.x};
}

std::vector<Move> Game::possible_moves(Square square) const {
	Piece piece = board[square.row][square.col];
	std::vector<Move> moves{};
	moves.reserve(8);
	if (piece == Piece::WHITE_PAWN || piece == Piece::BLACK_PAWN) {
		int dir = static_cast<int>(piece_color(piece));
		moves.push_back(Move{ square, Square{square.row, square.col + (1*dir)} });
		if ((square.row == 6 && piece == Piece::WHITE_PAWN) || (square.row == 1 && piece == Piece::BLACK_PAWN)) {
			moves.push_back(Move{ square, Square{square.row, square.col + (2*dir)} });
		}
	} else if (piece == Piece::WHITE_KNIGHT || piece == Piece::BLACK_KNIGHT) {
		std::array<Vec, 8> knight_moves{ Vec{-2, -1}, Vec{-1, -2}, Vec{1, -2}, Vec{2, -1}, Vec{2, 1}, Vec{1, 2}, Vec{-1, 2}, Vec{-2, 1} };
		for (Vec dir : knight_moves) {
			Square target = add(square, dir);
			if (is_within_board(target)) {
				moves.push_back(Move{square, target});
			}
		}
	} else if (piece == Piece::WHITE_BISHOP || piece == Piece::BLACK_BISHOP) {

	}*/
	return std::vector<Move>{};
	//return moves;
}

void Game::print() const {
	std::cout << "--------------------" << std::endl;
	for (const std::array<Piece, BOARD_SIZE>& row : board) {
		for (Piece piece : row) {
			std::cout << '|' << static_cast<char>(piece);
			if (piece == Piece::EMPTY) std::cout << ' ';
		}
		std::cout << "|\n";
	}
}

void Game::move(Move move) {
	board[move.to.row][move.to.col] = board[move.from.row][move.from.col];
	board[move.from.row][move.from.col] = Piece::EMPTY;
}
