#pragma once
#include "board.h"


class Game {
public:
	Game();
	Board* board = new Board();
	void update();
private:
	Color turn = Color::WHITE;
};