#include "Game.h"

Game::Game() { 
}

Color piece_color(Piece piece) {
	if (piece == Piece::EMPTY) return Color::NONE;
	if ((char)piece > 'a') return Color::BLACK;
	return Color::WHITE;
}

void Game::fen_interpreter(std::string fen) {
	// TODO: Do more than just set the board (set which sides turn it is etc.)
	int board_index = 0;
	for (int i = 0; i < fen.length(); i++) {
		if (fen[i] == ' ') break;
		if (fen[i] == '/') continue;
		if (isdigit(fen[i])) {
			board_index += (int)fen[i] - (int)'0';
		} else {
			board[board_index] = static_cast<Piece>(fen[i]);
			board_index++;
		}
	}
}

std::string Game::fen_generator() {
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

std::vector<Move> Game::possible_moves() {
	std::vector<Move> moves{};
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (board[i] == Piece::WHITE_PAWN) {
			moves.push_back(Move{ i, i-8 });
		}
		//if (piece_color(board[i]) == turn) {}
	}
	return moves;
}
