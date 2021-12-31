#pragma once

#include <array>
#include <vector>
#include <string>
#include "Board.h"

class Game {
public:
	Game();
	void fen_interpreter(const std::string&);
	std::string fen_generator() const;
	std::vector<Move> possible_moves(const Square&) const;
	Square get_square_from_name(const std::string&);
	void move(Move);
	Board* board = new Board();
private:
	Color turn = Color::WHITE;
	std::vector<Move> moves{};
};
