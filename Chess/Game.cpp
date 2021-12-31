#include "Game.h"
#include <iostream>
#include <tuple>

Game::Game() {
}

void Game::fen_interpreter(const std::string& fen){
	board->fen_interpreter(fen);
}

std::string Game::fen_generator() const{
	return board->fen_generator();
}

std::vector<Move> Game::possible_moves(const Square& square) const{
	return board->possible_moves(square);
}

Square Game::get_square_from_name(const std::string& name) {
	int row = 7 - name[1] + '1';
	int col = name[0] - 'a';
	return Square{col, row};
}

void Game::move(Move move) {
	board->move(move);
}