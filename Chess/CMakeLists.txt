# CMakeList.txt : CMake project for Chess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Chess "Chess.cpp" "Chess.h" "Game.cpp" "Game.h" "Board.cpp" "Board.h")

# TODO: Add tests and install targets if needed.
