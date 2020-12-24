# CMakeList.txt : CMake project for 2048, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (2048 "Classes/Game.cpp" "Classes/Game.h" "main.cpp" "Classes/HighScore.cpp" "Classes/HighScore.h" "Classes/UI.cpp" "Classes/UI.h")

# TODO: Add tests and install targets if needed.
target_link_libraries(2048 ncursesw)