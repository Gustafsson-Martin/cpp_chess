// Chess.cpp : Defines the entry point for the application.

#include "Chess.h"
#include "Game.h"
#include <vector>


int main() {
	Game game{};
	game.fen_interpreter("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << game.fen_generator() << std::endl;
	std::vector<Move> moves = game.possible_moves();
	for (int i = 0; i < moves.size(); i++) {
		std::cout << moves[i].from << " -> " << moves[i].to << std::endl;
	}
	return 0;
}
