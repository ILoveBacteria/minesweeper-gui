# Minesweeper GUI

## Description

This is my project for my basic programming course at Shahid Beheshti University.

## Libraries

This program uses [sdl](https://github.com/libsdl-org/SDL) and [SBDL](https://github.com/SBU-CE/SBDL) library

## Compile

1. Download SBDL from [here](https://github.com/SBU-CE/SBDL)
2. Download sdl and move the folder including *SDL2, SDL2_image, SDL2_mixer, SDL2_ttf* to your C drive
3. Here is a sample CMake:

```CMake
cmake_minimum_required(VERSION 3.20)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 11)

include_directories(Minesweeper C:/SDL/SDL2/include)
include_directories(Minesweeper C:/SDL/SDL2_image/include)
include_directories(Minesweeper C:/SDL/SDL2_mixer/include)
include_directories(Minesweeper C:/SDL/SDL2_ttf/include)
link_directories(Minesweeper C:/SDL/SDL2/lib/x86)
link_directories(Minesweeper C:/SDL/SDL2_image/lib/x86)
link_directories(Minesweeper C:/SDL/SDL2_mixer/lib/x86)
link_directories(Minesweeper C:/SDL/SDL2_ttf/lib/x86)

add_executable(Minesweeper main.cpp)

target_link_libraries(Minesweeper -std=c++11 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
```