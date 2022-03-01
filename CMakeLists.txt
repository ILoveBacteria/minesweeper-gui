cmake_minimum_required(VERSION 3.20)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 11)

include_directories(Minesweeper D:/SDL/SDL2/include)
include_directories(Minesweeper D:/SDL/SDL2_image/include)
include_directories(Minesweeper D:/SDL/SDL2_mixer/include)
include_directories(Minesweeper D:/SDL/SDL2_ttf/include)
link_directories(Minesweeper D:/SDL/SDL2/lib/x86)
link_directories(Minesweeper D:/SDL/SDL2_image/lib/x86)
link_directories(Minesweeper D:/SDL/SDL2_mixer/lib/x86)
link_directories(Minesweeper D:/SDL/SDL2_ttf/lib/x86)

add_executable(Minesweeper main.cpp)

target_link_libraries(Minesweeper -std=c++11 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)

