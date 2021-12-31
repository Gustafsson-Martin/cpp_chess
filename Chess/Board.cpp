#include "Board.h"
#include <iostream>

Board::Board() {}

Color operator-(Color c) {
	return static_cast<Color>(-static_cast<int>(c));
}

Square operator+(const Square& square, const Vec& vec) {
	return Square{square.col + vec.x, square.row + vec.y};
}

Color piece_color(Piece piece) {
	if (piece == Piece::EMPTY) return Color::NONE;
	if (static_cast<char>(piece) > 'a') return Color::BLACK;
	return Color::WHITE;
}

PieceType piece_type(Piece piece) {
	char type = static_cast<char>(piece);
	if (type < 'a') type += 'a' - 'A';
	return static_cast<PieceType>(type);
}

bool Board::is_within_board(const Square& square) const {
	return (square.row >= 0 && square.row < BOARD_LENGTH) && (square.col >= 0 && square.col < BOARD_LENGTH);
}

bool Board::is_legal_move(const Move& move) const {
	return is_within_board(move.to) && piece_color(at(move.from)) != piece_color(at(move.to));
}

bool Board::is_taking_move(const Move& move) const {
	return piece_color(at(move.from)) == -piece_color(at(move.to));
}

void Board::print() const {
	print_possible_moves(std::vector<Move>{});
}

void Board::print_possible_moves(std::vector<Move> moves) const {
	std::cout << "------------------- \n";
	for (int row = 0; row < BOARD_LENGTH; row++) {
		std::cout << std::to_string(8 - row) << ' ';
		for (int col = 0; col < BOARD_LENGTH; col++) {
			std::cout << '|';
			auto it = std::find_if(std::begin(moves), std::end(moves), [&row, &col](Move move) { return move.to.col == col && move.to.row == row; });
			if (it != std::end(moves)) {
				std::cout << 'x';
			}
			else {
				std::cout << static_cast<char>(board[row][col]);
				if (board[row][col] == Piece::EMPTY) std::cout << ' ';
			}
		}
		std::cout << "|\n";
	}
	std::cout << "   a b c d e f g h \n";
}

Piece& Board::operator[](const Square& square) {
	return board[square.row][square.col];
}

Piece Board::at(const Square& square) const {
	return board[square.row][square.col];
}

std::array<Piece, 8>& Board::operator[](const int row) {
	return board[row];
}

void Board::move(Move move) {
	(*this)[move.to] = (*this)[move.from];
	(*this)[move.from] = Piece::EMPTY;
}

std::vector<Move> Board::possible_moves(const Square& square) {
	Piece piece = at(square);
	PieceType type = piece_type(piece);
	std::vector<Move> moves{};
	moves.reserve(8);
	if (type == PieceType::PAWN) {
		int dir = static_cast<int>(piece_color(piece));
		if (board[square.row + dir][square.col] == Piece::EMPTY) {
			moves.push_back(Move{square, Square{square.col, square.row + dir}});
			if ((square.row == 6 && piece == Piece::WHITE_PAWN) || (square.row == 1 && piece == Piece::BLACK_PAWN)) {
				moves.push_back(Move{square, Square{square.col, square.row + (2 * dir)}});
			}
		}
		for (int i : std::array<int, 2>{-1, 1}) { // Check for taking moves left and right
			Move move = Move{square, Square{square.col + i, square.row + dir}};
			if (is_taking_move(move)) {
				moves.push_back(move);
			}
		}
	}
	else if (type == PieceType::KNIGHT) {
		std::array<Vec, 8> knight_moves{Vec{-2, -1}, Vec{-1, -2}, Vec{1, -2}, Vec{2, -1}, Vec{2, 1}, Vec{1, 2}, Vec{-1, 2}, Vec{-2, 1}};
		for (Vec dir : knight_moves) {
			Move move = Move{square, square + dir};
			if (is_legal_move(move)) {
				moves.push_back(move);
			}
		}
	}
	else if (type == PieceType::BISHOP || type == PieceType::QUEEN) {
		using pair = std::tuple<int, int>; // TODO: Make class or something
		for (const auto& [a, b] : std::array<pair, 4>{pair{-1, -1}, pair{1, -1}, pair{-1, 1}, pair{1, 1}}) {
			for (int i = 1; i < BOARD_LENGTH; i++) {
				Move move = Move{square, square + Vec{ a * i, b * i }};
				if (!is_legal_move(move)) break;
				moves.push_back(move);
				if (is_taking_move(move)) break;
			}
		}
	}
	else if (type == PieceType::KING) {
		std::array<Vec, 8> king_moves{Vec{-1, -1}, Vec{-1, 0}, Vec{-1, 1}, Vec{0, -1}, Vec{0, 1}, Vec{1, -1}, Vec{1, 0}, Vec{1, 1}};
		for (Vec dir : king_moves) {
			Move move = Move{square, square + dir};
			if (is_legal_move(move)) {
				moves.push_back(move);
			}
		}
	}
	if (type == PieceType::ROOK || type == PieceType::QUEEN) {
		using pair = std::tuple<int, int>;
		for (const auto& [a, b] : std::array<pair, 4>{pair{-1, 0}, pair{1, 0}, pair{0, -1}, pair{0, 1}}) {
			for (int i = 1; i < BOARD_LENGTH; i++) {
				Move move = Move{square, square + Vec{a * i, b * i}};
				if (!is_legal_move(move)) break;
				moves.push_back(move);
				if (is_taking_move(move)) break;
			}
		}
	}
	return moves;
}

void Board::fen_interpreter(const std::string& fen) {
	// TODO: Do more than just set the board (set which sides turn it is etc.)
	int board_index = 0;
	for (const char& chr : fen) {
		if (chr == ' ') break;
		if (chr == '/') continue;
		if (isdigit(chr)) {
			board_index += static_cast<int>(chr) - '0';
		}
		else {
			board[board_index / BOARD_LENGTH][board_index % BOARD_LENGTH] = static_cast<Piece>(chr);
			board_index++;
		}
	}
}

std::string Board::fen_generator() const {
	std::string fen{};
	int empty_counter{};
	for (const std::array<Piece, BOARD_LENGTH>& row : board) {
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


