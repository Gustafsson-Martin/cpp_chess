#include "Game.h"
#include <iostream>

Game::Game() {
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
			board[board_index] = static_cast<Piece>(chr);
			board_index++;
		}
	}
}

std::string Game::fen_generator() const {
	std::string fen{};
	int empty_counter{};
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (board[i] == Piece::EMPTY) empty_counter++;
		else {
			if (empty_counter > 0) fen += std::to_string(empty_counter);
			empty_counter = 0;
			fen += static_cast<char>(board[i]);
		}
		if ((i+1) % 8 == 0) {
			if (empty_counter > 0) fen += std::to_string(empty_counter);
			empty_counter = 0;
			if (i != 63) fen += "/";
		}
	}
	fen += " w - - 0 1";
	return fen;
}

std::vector<Move> Game::possible_moves(int index) const {
	Piece piece = board[index];
	std::vector<Move> moves{};
	if (piece == Piece::WHITE_PAWN || piece == Piece::BLACK_PAWN) {
		int dir = static_cast<int>(piece_color(piece));
		moves.push_back(Move{index, index + (dir*8)});
		if (index / 8 == 6 && piece == Piece::WHITE_PAWN) moves.push_back(Move{index, index + (dir*16)});
		if (index / 8 == 1 && piece == Piece::BLACK_PAWN) moves.push_back(Move{index, index + (dir*16)});
	} else if (piece == Piece::WHITE_KNIGHT || piece == Piece::BLACK_KNIGHT) {
	}
	return moves;
}

void Game::move(Move move) {
	board[move.to] = board[move.from];
	board[move.from] = Piece::EMPTY;
}
