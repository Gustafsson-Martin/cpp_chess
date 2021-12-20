// Chess.cpp : Defines the entry point for the application.

#include "Chess.h"
#include "Game.h"
#include <vector>


int main() {
	Game game{};
	game.fen_interpreter("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::vector<Move> moves = game.possible_moves(Square{6,5});
	for (int i = 0; i < moves.size(); i++) {
		std::cout << moves[i].from.row << moves[i].from.col << " -> " << moves[i].to.row << moves[i].to.col << std::endl;
	}
	game.print_possible_moves(moves);
	//game.move(moves[10]);
	std::cout << game.fen_generator() << std::endl;
	game.print();
	return 0;
}
