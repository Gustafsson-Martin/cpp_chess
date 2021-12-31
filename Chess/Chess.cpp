// Chess.cpp : Defines the entry point for the application.

#include "Chess.h"
#include "Game.h"
#include <vector>


int main() {
	Game game{};
	game.fen_interpreter("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	while (true) {
		game.board->print();
		std::string square_name{};
		std::cin >> square_name;
		Square square = game.get_square_from_name(square_name);
		std::vector<Move> moves = game.possible_moves(square);
		game.board->print_possible_moves(moves);
		int i;
		std::cin >> i;
		if (i >= moves.size()) continue;
		Move move = moves[i];
		game.move(move);
	}
	return 0;
}
