#include "board.h"

Color operator-(Color c) {
	return static_cast<Color>(-static_cast<int>(c));
}

Square operator+(const Square& square, const Vec& vec) {
	return Square{ square.row + vec.y, square.col + vec.x };
}

bool operator==(const Square& current, const Square& other) {
	return current.row == other.row && current.col == other.col;
}

Color piece_color(Piece piece) {
	if (piece == Piece::NONE) return Color::NONE;
	if (static_cast<char>(piece) > 'a') return Color::BLACK;
	return Color::WHITE;
}

PieceType piece_type(Piece piece) {
	char type = static_cast<char>(piece);
	if (type < 'a') type += 'a' - 'A';
	return static_cast<PieceType>(type);
}

Board::Board() {
	reset_board();
	load_textures();
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
	(*this)[move.from] = Piece::NONE;
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

std::vector<Move> Board::possible_moves(const Square& square) {
	std::vector<Move> moves{};
	if (square.row == 8) return moves;
	Piece piece = at(square);
	PieceType type = piece_type(piece);
	moves.reserve(8);
	if (type == PieceType::PAWN) {
		int dir = static_cast<int>(piece_color(piece));
		if (board[square.row + dir][square.col] == Piece::NONE) {
			moves.push_back(Move{ square, Square{square.row + dir, square.col} });
			if ((square.row == 6 && piece == Piece::WHITE_PAWN) || (square.row == 1 && piece == Piece::BLACK_PAWN)) {
				moves.push_back(Move{ square, Square{square.row + (2 * dir), square.col} });
			}
		}
		for (int i : std::array<int, 2>{-1, 1}) { // Check for taking moves left and right
			Move move = Move{ square, Square{square.row + dir, square.col + i} };
			if (is_legal_move(move) && is_taking_move(move)) {
				moves.push_back(move);
			}
		}
	}
	else if (type == PieceType::KNIGHT) {
		std::array<Vec, 8> knight_moves{ Vec{-2, -1}, Vec{-1, -2}, Vec{1, -2}, Vec{2, -1}, Vec{2, 1}, Vec{1, 2}, Vec{-1, 2}, Vec{-2, 1} };
		for (Vec dir : knight_moves) {
			Move move = Move{ square, square + dir };
			if (is_legal_move(move)) {
				moves.push_back(move);
			}
		}
	}
	else if (type == PieceType::BISHOP || type == PieceType::QUEEN) {
		using pair = std::tuple<int, int>; // TODO: Make class or something
		for (const auto& [a, b] : std::array<pair, 4>{pair{ -1, -1 }, pair{ 1, -1 }, pair{ -1, 1 }, pair{ 1, 1 }}) {
			for (int i = 1; i < BOARD_LENGTH; i++) {
				Move move = Move{ square, square + Vec{ a * i, b * i } };
				if (!is_legal_move(move)) break;
				moves.push_back(move);
				if (is_taking_move(move)) break;
			}
		}
	}
	else if (type == PieceType::KING) {
		std::array<Vec, 8> king_moves{ Vec{-1, -1}, Vec{-1, 0}, Vec{-1, 1}, Vec{0, -1}, Vec{0, 1}, Vec{1, -1}, Vec{1, 0}, Vec{1, 1} };
		for (Vec dir : king_moves) {
			Move move = Move{ square, square + dir };
			if (is_legal_move(move)) {
				moves.push_back(move);
			}
		}
	}
	if (type == PieceType::ROOK || type == PieceType::QUEEN) {
		using pair = std::tuple<int, int>;
		for (const auto& [a, b] : std::array<pair, 4>{pair{ -1, 0 }, pair{ 1, 0 }, pair{ 0, -1 }, pair{ 0, 1 }}) {
			for (int i = 1; i < BOARD_LENGTH; i++) {
				Move move = Move{ square, square + Vec{a * i, b * i} };
				if (!is_legal_move(move)) break;
				moves.push_back(move);
				if (is_taking_move(move)) break;
			}
		}
	}
	return moves;
}

bool Board::is_possible_move(const std::vector<Move>& moves, const Square& square) {
	for (const Move& move : moves) {
		if (square == move.to) return true;
	}
	return false;
}

void Board::draw() {
	float cell_size = std::min(ImGui::GetWindowWidth() * 0.11f, ImGui::GetWindowHeight() * 0.11f);
	ImVec2 image_size = ImVec2(cell_size, cell_size);
	ImVec4 black_cell_color = ImVec4(118.0f / 256.0f, 150.0f / 256.0f, 86.0f / 256.0f, 1.0f);
	ImVec4 white_cell_color = ImVec4(238.0f / 256.0f, 238.0f / 256.0f, 210.0f / 256.0f, 1.0f);
	ImVec4 selected_color = ImVec4(0.8f, 0.8f, 0.0f, 0.8f);
	ImVec4 possible_move_color = ImVec4(0.0f, 0.2f, 0.6f, 0.2f);
	ImVec4 default_tint = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	std::vector<Move> moves = possible_moves(selected);

	/* Generate 8x8 buttons for the board */
	for (int row = 0; row < 8; row++) {
		ImGui::SameLine();
		ImGui::Text(std::to_string(8 - row).c_str());
		for (int col = 0; col < 8; col++) {
			ImGui::PushID(row * 8 + col);
			ImGui::SameLine();
			ImVec4 background_color = ((row + col) % 2 == 0) ? white_cell_color : black_cell_color;
			if (selected.row == row && selected.col == col) background_color = selected_color;
			for (const Move& move : moves) {
				if (move.to == Square{row, col}) {
					//background_color = possible_move_color;
					background_color.w = 0.6f;
				}
			}
			ImVec4 tint = board[row][col] == Piece::NONE ? ImVec4(1.0f, 1.0f, 1.0f, 0.0f) : default_tint;

			if (ImGui::ImageButton(textures[board[row][col]], image_size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 0, background_color, tint)) {
				std::cout << "pressed row = " << row << " col = " << col << std::endl;
				if (selected.row == 8 || board[selected.row][selected.col] == Piece::NONE) {
					selected = { row, col };
					std::cout << "here1" << std::endl;
				} else {
					std::cout << "here2" << std::endl;
					if (is_possible_move(moves, Square{ row, col })) {
						board[row][col] = board[selected.row][selected.col];
						board[selected.row][selected.col] = Piece::NONE;
					}
					selected = { 8, 8 };
				}
			}
			ImGui::PopID();
		}
		ImGui::NewLine();
	}

	ImGui::PopStyleVar();
}

void Board::reset_board() {
	static const std::string starting_position{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	fen_interpreter(starting_position);
}

void Board::fen_interpreter(const std::string& fen) {
	/* https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation */
	// TODO: Do more than just set the board (set which sides turn it is etc.)
	int board_index = 0;
	for (const char& chr : fen) {
		if (chr == ' ') break;
		if (chr == '/') continue;
		if (isdigit(chr)) {
			board_index += static_cast<int>(chr) - '0';
		} else {
			board[board_index / BOARD_LENGTH][board_index % BOARD_LENGTH] = static_cast<Piece>(chr);
			board_index++;
		}
	}
}

void Board::load_textures() {
	if (!textures.empty()) return;
	int width{}, height{};
	GLuint texture{};
	for (const auto& [piece, file] : texture_files) {
		bool success = LoadTextureFromFile(file.c_str(), &texture, &width, &height);
		if (!success) throw std::runtime_error("loading texture from file " + file + " failed");
		textures[piece] = (void*)(intptr_t)texture;
	}
}