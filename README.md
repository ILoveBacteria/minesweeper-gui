# Minesweeper GUI

## Description

This is my project for my basic programming course at Shahid Beheshti University.


![App screenshots1](https://github.com/ILoveBacteria/minesweeper-gui/blob/master/Assets/preview2.png)
![App screenshots2](https://github.com/ILoveBacteria/minesweeper-gui/blob/master/Assets/preview3.png)

## Libraries

This program uses [sdl](https://github.com/libsdl-org/SDL) and [SBDL](https://github.com/SBU-CE/SBDL) library

## Compile And Run

1. Clone this project:

```sh
$ git clone https://github.com/ILoveBacteria/minesweeper-gui.git
```

3. Download SBDL library from [here](https://github.com/SBU-CE/SBDL) and read the tutorial
4. Here is a sample CMake:

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

5. Compile the project and run

## Last Release

You can download and run the last release from [here](https://github.com/ILoveBacteria/minesweeper-gui/releases)
