#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

#include "types.h"
#include "textureloading.h"
#include "imgui.h"

const std::unordered_map<Piece, std::string> texture_files = {
	{Piece::WHITE_PAWN, "assets/white_pawn.png"},
	{Piece::WHITE_ROOK, "assets/white_rook.png"},
	{Piece::WHITE_KNIGHT, "assets/white_knight.png"},
	{Piece::WHITE_BISHOP, "assets/white_bishop.png"},
	{Piece::WHITE_QUEEN, "assets/white_queen.png"},
	{Piece::WHITE_KING, "assets/white_king.png"},
	{Piece::BLACK_PAWN, "assets/black_pawn.png"},
	{Piece::BLACK_ROOK, "assets/black_rook.png"},
	{Piece::BLACK_KNIGHT, "assets/black_knight.png"},
	{Piece::BLACK_BISHOP, "assets/black_bishop.png"},
	{Piece::BLACK_QUEEN, "assets/black_queen.png"},
	{Piece::BLACK_KING, "assets/black_king.png"},
};


struct Square {
	int row;
	int col;
};

struct Move {
	Square from;
	Square to;
};

struct Vec {
	int x;
	int y;
};


class Board {
public:
	Board();
	void reset_board();
	void fen_interpreter(const std::string&);
	void draw();
	static constexpr int BOARD_LENGTH{ 8 };
	BoardType board{};
	std::vector<Move> possible_moves(const Square&);
	Piece& operator[](const Square&);
	Piece at(const Square& square) const;
	std::array<Piece, 8>& operator[](const int);
	void move(Move move);
private:
	void load_textures();
	std::unordered_map<Piece, void*> textures;
	Square selected = {8, 8};
	bool is_within_board(const Square& square) const;
	bool is_legal_move(const Move& move) const;
	bool is_taking_move(const Move& move) const;
	bool is_possible_move(const std::vector<Move>&, const Square&);
};