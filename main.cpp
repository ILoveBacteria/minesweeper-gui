#include "SBDL.h"
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

#define MOUSE_LEFT_CLICKED(rect) (SBDL::mouseInRect(rect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
#define MOUSE_RIGHT_CLICKED(rect) (SBDL::mouseInRect(rect) && SBDL::Mouse.clicked(SDL_BUTTON_RIGHT, 1, SDL_PRESSED))

#define POINT game.countSquareInRow * game.countBombs
#define LEFT -1
#define RIGHT 1
struct Player {
    std::string score;
    std::string id;
};

struct Save {
    std::string countSquaresInRow;
    std::string countBombs;
    std::string time;
    std::string coverSquares;
    std::string flag;
    std::string bomb;
    std::string id;
};

struct Game {
    int countBombs;
    int countSquareInRow;
    Player player;
    struct Square {
        SDL_Rect **backgroundSquare;
        SDL_Rect ***bombs;
        SDL_Rect ***coverSquare;
        SDL_Rect ***flag;
        SDL_Rect ***number0;
        SDL_Rect ***number1;
        SDL_Rect ***number2;
        SDL_Rect ***number3;
        SDL_Rect ***number4;
        SDL_Rect ***number5;
        SDL_Rect ***number6;
        SDL_Rect ***number7;
        SDL_Rect ***number8;
    }square;
}game;

enum Window {
    LOGIN, MAIN, DIFFICULTY, MENU, LEADERBOARD, CHANGE_NAME, LOAD_GAME, GAME_OVER, WIN
}window;

enum Difficultly {
    EASY, NORMAL, HARD, CUSTOM
};

Texture checkButton1, checkButton2, enterButton1, enterButton2, arrow, addUserButton1, addUserButton2, trashButton1,
        trashButton2, cancelButton1, cancelButton2, searchButton1, searchButton2, bombTexture, number1, number2,
        number3, number4, number5, number6, number7, number8, flag, saveButton1, saveButton2, down1, down2, up1, up2;
Font *font1, *font2, *font3;

void LoadTextures() {
    checkButton1 = SBDL::loadTexture("Reference/check1.png");
    checkButton2 = SBDL::loadTexture("Reference/check2.png");
    enterButton1 = SBDL::loadTexture("Reference/enter1.png");
    enterButton2 = SBDL::loadTexture("Reference/enter2.png");
    arrow = SBDL::loadTexture("Reference/arrow.png");
    down1 = SBDL::loadTexture("Reference/down1.png");
    down2 = SBDL::loadTexture("Reference/down2.png");
    up1 = SBDL::loadTexture("Reference/up1.png");
    up2 = SBDL::loadTexture("Reference/up2.png");
    addUserButton1 = SBDL::loadTexture("Reference/user-add1.png");
    addUserButton2 = SBDL::loadTexture("Reference/user-add2.png");
    trashButton1 = SBDL::loadTexture("Reference/trash1.png");
    trashButton2 = SBDL::loadTexture("Reference/trash2.png");
    cancelButton1 = SBDL::loadTexture("Reference/remove1.png");
    cancelButton2 = SBDL::loadTexture("Reference/remove2.png");
    searchButton1 = SBDL::loadTexture("Reference/search1.png");
    searchButton2 = SBDL::loadTexture("Reference/search2.png");
    bombTexture = SBDL::loadTexture("Reference/bomb.png");
    number1 = SBDL::loadTexture("Reference/1.png");
    number2 = SBDL::loadTexture("Reference/2.png");
    number3 = SBDL::loadTexture("Reference/3.png");
    number4 = SBDL::loadTexture("Reference/4.png");
    number5 = SBDL::loadTexture("Reference/5.png");
    number6 = SBDL::loadTexture("Reference/6.png");
    number7 = SBDL::loadTexture("Reference/7.png");
    number8 = SBDL::loadTexture("Reference/8.png");
    flag = SBDL::loadTexture("Reference/flag.png");
    saveButton1 = SBDL::loadTexture("Reference/save1.png");
    saveButton2 = SBDL::loadTexture("Reference/save2.png");
}

void LoadFonts() {
    font1 = SBDL::loadFont("Reference/comic.ttf", 20);
    font2 = SBDL::loadFont("Reference/ITCKRIST.TTF", 20);
    font3 = SBDL::loadFont("Reference/comic.ttf", 30);
}

int pow(int n, int power) {
    int result = 1;

    for (int i = 0; i < power; ++i)
        result *= n;

    return result;
}

int StrToNum(std::string &str) {
    int length = str.length();
    int result = 0;

    for (int i = 0, power = length-1; i < length; ++i) {
        result += (str[i]-48) * pow(10, power);
        power--;
    }

    return result;
}

void Keyboard(std::string &str, bool onlyNumber = false) {
    if (SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)) {
        str.pop_back();
        if (str.length() == 0)
            str.push_back(' ');
        str.shrink_to_fit();
    }
    else if (onlyNumber) {
        if (SBDL::keyPressed(SDL_SCANCODE_KP_0) || SBDL::keyPressed(SDL_SCANCODE_0)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('0');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_1) || SBDL::keyPressed(SDL_SCANCODE_1)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('1');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_2) || SBDL::keyPressed(SDL_SCANCODE_2)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('2');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_3) || SBDL::keyPressed(SDL_SCANCODE_3)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('3');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_4) || SBDL::keyPressed(SDL_SCANCODE_4)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('4');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_5) || SBDL::keyPressed(SDL_SCANCODE_5)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('5');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_6) || SBDL::keyPressed(SDL_SCANCODE_6)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('6');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_7) || SBDL::keyPressed(SDL_SCANCODE_7)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('7');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_8) || SBDL::keyPressed(SDL_SCANCODE_8)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('8');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_KP_9) || SBDL::keyPressed(SDL_SCANCODE_9)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('9');
        }

    } else {
        if (SBDL::keyPressed(SDL_SCANCODE_A)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('a');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_B)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('b');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_C)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('c');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_D)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('d');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_E)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('e');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_F)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('f');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_G)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('g');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_H)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('h');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_I)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('i');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_J)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('j');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_K)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('k');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_L)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('l');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_M)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('m');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_N)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('n');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_O)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('o');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_P)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('p');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_Q)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('q');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_R)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('r');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_S)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('s');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_T)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('t');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_U)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('u');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_V)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('v');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_W)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('w');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_X)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('x');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_Y)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('y');
        }

        else if (SBDL::keyPressed(SDL_SCANCODE_Z)) {
            if (str[0] == ' ')
                str.pop_back();
            str.push_back('z');
        }
    }
}

Save* ReadSaveSlots() {
    // Reading saves from file
    std::ifstream read;
    std::string line;
    read.open("save.txt");

    std::getline(read, line);
    int countSaves = StrToNum(line);

    auto *p_save = new Save[countSaves];

    for (int i = 0; i < countSaves; ++i) {
        std::getline(read, (p_save + i)->countSquaresInRow);
        std::getline(read, (p_save + i)->countBombs);
        std::getline(read, (p_save + i)->time);
        std::getline(read, (p_save + i)->coverSquares);
        std::getline(read, (p_save + i)->flag);
        std::getline(read, (p_save + i)->bomb);
        std::getline(read, (p_save + i)->id);
    }
    read.close();

    return countSaves == 0? nullptr: p_save;
}

void DeleteSaveSlot(Save *&p_saveSlot, int index) {
    int countSaves;
    // Read number of saves
    std::ifstream read("save.txt");
    std::string line;
    std::getline(read, line);
    countSaves = StrToNum(line);
    read.close();

    // Writing on a text file
    std::ofstream write ("save.txt");

    write << countSaves - 1 << '\n';
    for (int i = 0; i < countSaves; ++i) {
        if (i == index)
            continue;

        write << (p_saveSlot + i)->countSquaresInRow << '\n';
        write << (p_saveSlot + i)->countBombs << '\n';
        write << (p_saveSlot + i)->time << '\n';
        write << (p_saveSlot + i)->coverSquares << '\n';
        write << (p_saveSlot + i)->flag << '\n';
        write << (p_saveSlot + i)->bomb << '\n';
        write << (p_saveSlot + i)->id << '\n';
    }
    write.close();

    delete[] p_saveSlot;
    p_saveSlot = ReadSaveSlots();
}

int CountFlags() {
    int result = 0;
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.flag[i][j] != nullptr)
                ++result;
        }
    }

    return result;
}

std::string NumToStr(int n) {
    int length = 0;
    std::string result;

    // 0
    if (n == 0) {
        result.push_back('0');
        return result;
    }

    // Length of number
    for (int i = 0;; ++i) {
        if (n / pow(10, i) > 0)
            ++length;
        else
            break;
    }

    // Converting to string
    for (int i = length-1; i >= 0; --i) {
        result.push_back((n / pow(10, i)) % 10 + 48);
    }

    return result;
}

std::string CurrentTime() {
    // Current date/time based on current system
    time_t now = time(nullptr);
    // convert now to string form
    return ctime(&now);
}

std::string ConvertToSavable(SDL_Rect ***a) {
    std::string result;
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (a[i][j] == nullptr)
                result.push_back('0');
            else
                result.push_back('1');
        }
    }

    return result;
}

bool IsFlagOn(int row, int column) {
    if (game.square.flag[row][column] != nullptr)
        return true;

    return false;
}

int CountOpenedSquares() {
    int result = 0;
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.coverSquare[i][j] == nullptr)
                ++result;
        }
    }

    return result;
}

int OpenSquare(int row, int column) {
    /**
     * 0 = has already opened
     * 1 = exit function or opened successfully
     * 2 = bomb under the square
     */

    // avoid AIOOB
    if (row < 0 || column < 0 || row >= game.countSquareInRow || column >= game.countSquareInRow ||
        game.square.coverSquare[row][column] == nullptr)
        return 0;

    // bomb?
    if (game.square.bombs[row][column] != nullptr)
        return 2;

    bool nearSquareIsBomb = true;
    if (game.square.number0[row][column] != nullptr) {
        nearSquareIsBomb = false;
    }

    if (nearSquareIsBomb) {
        // Remove cover square
        game.square.coverSquare[row][column] = nullptr;
        // Remove flag
        game.square.flag[row][column] = nullptr;
        return 1;

    } else {
        // Remove cover square
        game.square.coverSquare[row][column] = nullptr;
        // Remove flag
        game.square.flag[row][column] = nullptr;

        // Recursively opening other empty squares
        OpenSquare(row - 1, column - 1);
        OpenSquare(row - 1, column);
        OpenSquare(row - 1, column + 1);
        OpenSquare(row, column - 1);
        OpenSquare(row, column + 1);
        OpenSquare(row + 1, column - 1);
        OpenSquare(row + 1, column);
        OpenSquare(row + 1, column + 1);
    }
    return 1;
}

void SaveGame() {
    // Reading saves from file
    std::ifstream read;
    std::string line;
    read.open("save.txt");

    std::getline(read, line);
    int countSaves = StrToNum(line);

    auto *p_save = new Save[countSaves];

    for (int i = 0; i < countSaves; ++i) {
        std::getline(read, (p_save + i)->countSquaresInRow);
        std::getline(read, (p_save + i)->countBombs);
        std::getline(read, (p_save + i)->time);
        std::getline(read, (p_save + i)->coverSquares);
        std::getline(read, (p_save + i)->flag);
        std::getline(read, (p_save + i)->bomb);
        std::getline(read, (p_save + i)->id);
    }
    read.close();

    std::ofstream write;
    write.open("save.txt");

    write << countSaves + 1 << '\n';
    for (int i = 0; i < countSaves; ++i) {
        write << (p_save + i)->countSquaresInRow << '\n';
        write << (p_save + i)->countBombs << '\n';
        write << (p_save + i)->time << '\n';
        write << (p_save + i)->coverSquares << '\n';
        write << (p_save + i)->flag << '\n';
        write << (p_save + i)->bomb << '\n';
        write << (p_save + i)->id << '\n';
    }
    delete[] p_save;

    write << NumToStr(game.countSquareInRow) << '\n';
    write << NumToStr(game.countBombs) << '\n';
    write << CurrentTime();
    write << ConvertToSavable(game.square.coverSquare) << '\n';
    write << ConvertToSavable(game.square.flag) << '\n';
    write << ConvertToSavable(game.square.bombs) << '\n';
    write << game.player.id;

    write.close();
}

void InsertBombs() {
    for (int i = 0; i < game.countBombs; ++i) {
        int row = rand() % game.countSquareInRow;
        int column = rand() % game.countSquareInRow;

        // Avoiding duplicate random number
        if (game.square.bombs[row][column] != nullptr) {
            --i;
            continue;
        }

        // Assigning the memory address of Rect
        game.square.bombs[row][column] = &game.square.backgroundSquare[row][column];
    }
}

void CountOfBombsNearSquare() {
    int sum[20][20] = {0};

    /**
     *  ===row1===
     *  |        |
     *  4        2
     *  |        |
     *  ===row3===
     *
     */
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            bool checkRow1 = true;
            bool checkRow2 = true;
            bool checkRow3 = true;
            bool checkRow4 = true;

            // Bomb in this square?
            if (game.square.bombs[i][j] != nullptr) {
                if (i-1 < 0)
                    checkRow1 = false;
                else
                    ++sum[i-1][j];

                if (i+1 >= game.countSquareInRow)
                    checkRow3 = false;
                else
                    ++sum[i+1][j];

                if (j-1 < 0)
                    checkRow4 = false;
                else
                    ++sum[i][j-1];

                if (j+1 >= game.countSquareInRow)
                    checkRow2 = false;
                else
                    ++sum[i][j+1];

                // Corner squares
                if (checkRow1 && checkRow4)
                    ++sum[i-1][j-1];
                if (checkRow1 && checkRow2)
                    ++sum[i-1][j+1];
                if (checkRow2 && checkRow3)
                    ++sum[i+1][j+1];
                if (checkRow3 && checkRow4)
                    ++sum[i+1][j-1];
            }
        }
    }

    // Setting bomb squares to 0
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.bombs[i][j] != nullptr)
                sum[i][j] = -1;
        }
    }

    // Assigning square address
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (sum[i][j] == 0) {
                game.square.number0[i][j] = &game.square.backgroundSquare[i][j];
            }
            else if (sum[i][j] == 1) {
                game.square.number1[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 2) {
                game.square.number2[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 3) {
                game.square.number3[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 4) {
                game.square.number4[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 5) {
                game.square.number5[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 6) {
                game.square.number6[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 7) {
                game.square.number7[i][j] = &game.square.backgroundSquare[i][j];
            }

            else if (sum[i][j] == 8) {
                game.square.number8[i][j] = &game.square.backgroundSquare[i][j];
            }
        }
    }
}

void DeallocateGameMemory() {
    /**
    *  Deallocating memories
    *  For first time avoid deallocate
    */
    if (game.square.backgroundSquare != nullptr) {
        for (int i = 0; i < game.countSquareInRow; ++i) {
            delete[] game.square.backgroundSquare[i];
            delete[] game.square.coverSquare[i];
            delete[] game.square.flag[i];
            delete[] game.square.bombs[i];
            delete[] game.square.number0[i];
            delete[] game.square.number1[i];
            delete[] game.square.number2[i];
            delete[] game.square.number3[i];
            delete[] game.square.number4[i];
            delete[] game.square.number5[i];
            delete[] game.square.number6[i];
            delete[] game.square.number7[i];
            delete[] game.square.number8[i];
        }
        delete[] game.square.backgroundSquare;
        delete[] game.square.coverSquare;
        delete[] game.square.flag;
        delete[] game.square.bombs;
        delete[] game.square.number0;
        delete[] game.square.number1;
        delete[] game.square.number2;
        delete[] game.square.number3;
        delete[] game.square.number4;
        delete[] game.square.number5;
        delete[] game.square.number6;
        delete[] game.square.number7;
        delete[] game.square.number8;
    }
}

void AllocateGameMemory() {
    game.square.backgroundSquare = new SDL_Rect*[game.countSquareInRow];
    game.square.coverSquare = new SDL_Rect**[game.countSquareInRow];
    game.square.bombs = new SDL_Rect**[game.countSquareInRow];
    game.square.flag = new SDL_Rect**[game.countSquareInRow];
    game.square.number0 = new SDL_Rect**[game.countSquareInRow];
    game.square.number1 = new SDL_Rect**[game.countSquareInRow];
    game.square.number2 = new SDL_Rect**[game.countSquareInRow];
    game.square.number3 = new SDL_Rect**[game.countSquareInRow];
    game.square.number4 = new SDL_Rect**[game.countSquareInRow];
    game.square.number5 = new SDL_Rect**[game.countSquareInRow];
    game.square.number6 = new SDL_Rect**[game.countSquareInRow];
    game.square.number7 = new SDL_Rect**[game.countSquareInRow];
    game.square.number8 = new SDL_Rect**[game.countSquareInRow];

    for (int i = 0; i < game.countSquareInRow; ++i) {
        game.square.backgroundSquare[i] = new SDL_Rect[game.countSquareInRow];
        game.square.coverSquare[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.bombs[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.flag[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number0[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number1[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number2[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number3[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number4[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number5[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number6[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number7[i] = new SDL_Rect*[game.countSquareInRow];
        game.square.number8[i] = new SDL_Rect*[game.countSquareInRow];
    }

    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            game.square.number0[i][j] = nullptr;
            game.square.number1[i][j] = nullptr;
            game.square.number2[i][j] = nullptr;
            game.square.number3[i][j] = nullptr;
            game.square.number4[i][j] = nullptr;
            game.square.number5[i][j] = nullptr;
            game.square.number6[i][j] = nullptr;
            game.square.number7[i][j] = nullptr;
            game.square.number8[i][j] = nullptr;
            game.square.flag[i][j] = nullptr;
            game.square.bombs[i][j] = nullptr;
        }
    }
}

void CreateGameBoard() {
    int startPoint_x = 3, startPoint_y = 3;
    // Creating squares
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            game.square.backgroundSquare[i][j].x = startPoint_x;
            game.square.backgroundSquare[i][j].y = startPoint_y;
            game.square.backgroundSquare[i][j].w = 25;
            game.square.backgroundSquare[i][j].h = 25;

            // Copy square address
            game.square.coverSquare[i][j] = &game.square.backgroundSquare[i][j];

            startPoint_x += 28;
        }
        startPoint_x = 3;
        startPoint_y += 28;
    }
}

void LoadGame(Save *p_loadThis) {
    DeallocateGameMemory();
    game.countSquareInRow = StrToNum(p_loadThis->countSquaresInRow);
    game.countBombs = StrToNum(p_loadThis->countBombs);
    AllocateGameMemory();
    CreateGameBoard();

    for (int i = 0, k = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j, ++k) {
            if (p_loadThis->bomb[k] == '1')
                game.square.bombs[i][j] = &game.square.backgroundSquare[i][j];
            else
                game.square.bombs[i][j] = nullptr;
        }
    }

    for (int i = 0, k = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j, ++k) {
            if (p_loadThis->flag[k] == '1')
                game.square.flag[i][j] = &game.square.backgroundSquare[i][j];
            else
                game.square.flag[i][j] = nullptr;
        }
    }

    for (int i = 0, k = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j, ++k) {
            if (p_loadThis->coverSquares[k] == '0')
                game.square.coverSquare[i][j] = nullptr;
        }
    }

    CountOfBombsNearSquare();
}

void Swap(Player &a, Player &b) {
    Player temp = b;
    b = a;
    a = temp;
}

void BubbleSort(Player *a, const int SIZE) {
    for (int i = 0; i < SIZE - 1; ++i) {
        for (int j = 0; j < SIZE - i - 1; ++j) {
            if (StrToNum((a+j)->score) < StrToNum((a+j+1)->score)) {
                Swap(a[j], a[j+1]);
            }
        }
    }
}

Player* ReadPlayers() {
    std::ifstream read("player.txt");;
    std::string line;

    std::getline(read, line);

    int countPlayers = StrToNum(line);

    auto *p_player = new Player[countPlayers];

    for (int i = 0; i < countPlayers; ++i) {
        std::getline(read, (p_player + i)->score);
        std::getline(read, (p_player + i)->id);
    }
    read.close();
    return p_player;
}

int ReadNumberOfPlayers() {
    std::ifstream read("player.txt");;
    std::string line;
    std::getline(read, line);
    read.close();
    return StrToNum(line);
}

void LoginWindow(Player *&p_player) {
    SDL_Rect idFieldRect = {210, 285, 400, 70};
    SDL_Rect arrowRect = {215, 310, 20, 20};
    SDL_Rect searchButtonRect = {230, 375, 40, 40};
    SDL_Rect trashButtonRect = {290, 375, 40, 40};
    SDL_Rect addUserButtonRect = {350, 375, 40, 40};
    SDL_Rect enterButtonRect = {410, 375, 45, 45};

    int countPlayers = ReadNumberOfPlayers();
    static Texture loginPrompt;
    static std::string s_idPlayer = " ";
    static bool s_loginActive = false;
    Keyboard(s_idPlayer);

    // Clicked on add user button
    if (MOUSE_LEFT_CLICKED(addUserButtonRect) && s_idPlayer[0] != ' ') {
        bool idExist = false;
        for (int i = 0; i < countPlayers; ++i) {
            if (s_idPlayer == (p_player + i)->id) {
                idExist = true;
                break;
            }
        }

        if (!idExist) {
            // Writing on a text file
            std::ofstream write ("player.txt");

            write << countPlayers + 1 << '\n';
            for (int i = 0; i < countPlayers; ++i) {
                write << (p_player + i)->score << '\n';
                write << (p_player + i)->id << '\n';
            }
            write << "0" << '\n' << s_idPlayer;

            write.close();
            delete[] p_player;
            p_player = ReadPlayers();

            SBDL::freeTexture(loginPrompt);
            loginPrompt = SBDL::createFontTexture(font1, "Log in as: " + s_idPlayer, 5, 117, 40);

            game.player.id = s_idPlayer;
            game.player.score = "0";
            s_loginActive = true;
            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
        }
    }

    // Clicked on trash button
    else if (MOUSE_LEFT_CLICKED(trashButtonRect) && s_idPlayer[0] != ' ') {
        bool idExist = false;
        int idIndex;
        for (int i = 0; i < countPlayers; ++i) {
            if (s_idPlayer == (p_player + i)->id) {
                idExist = true;
                idIndex = i;
                break;
            }
        }

        if (idExist) {
            // Writing on a text file
            std::ofstream write ("player.txt");

            write << countPlayers - 1 << '\n';
            for (int i = 0; i < countPlayers; ++i) {
                if (i == idIndex)
                    continue;
                write << (p_player + i)->score << '\n';
                write << (p_player + i)->id << '\n';
            }

            write.close();
            delete[] p_player;
            p_player = ReadPlayers();

            if (s_idPlayer == game.player.id) {
                s_loginActive = false;
                SBDL::freeTexture(loginPrompt);
                loginPrompt = SBDL::createFontTexture(font1, " ", 25, 145, 125);
            }

            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
            s_loginActive = false;
        }
    }

    // Clicked on search button
    else if (MOUSE_LEFT_CLICKED(searchButtonRect) && s_idPlayer[0] != ' ') {
        bool idExist = false;
        int i;
        for (i = 0; i < countPlayers; ++i) {
            if (s_idPlayer == (p_player + i)->id) {
                idExist = true;
                break;
            }
        }

        if (idExist) {
            SBDL::freeTexture(loginPrompt);
            loginPrompt = SBDL::createFontTexture(font1, "Log in as: " + s_idPlayer, 5, 83, 117);

            game.player.id = s_idPlayer;
            game.player.score = "0";
            s_loginActive = true;
            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
        }
    }

    // Clicked on enter button
    else if (MOUSE_LEFT_CLICKED(enterButtonRect) && s_loginActive) {
        window = MENU;
        s_loginActive = false;
        s_idPlayer = " ";
        s_idPlayer.shrink_to_fit();
    }

    // Draw Rectangles
    SBDL::drawRectangle(idFieldRect,199, 255, 254);

    // Arrow
    if (SBDL::getTime() / 1000 % 2 == 0)
        SBDL::showTexture(arrow, arrowRect);

    // Enter Button
    if (SBDL::mouseInRect(enterButtonRect))
        SBDL::showTexture(enterButton2, enterButtonRect);
    else
        SBDL::showTexture(enterButton1, enterButtonRect);

    if (!s_loginActive)
        SBDL::drawRectangle(enterButtonRect, 255, 255, 255, 170);

    // ID inputting string
    Texture strID = SBDL::createFontTexture(font1, s_idPlayer, 0, 0, 0);
    SBDL::showTexture(strID, 240, 303);
    SBDL::freeTexture(strID);

    // Log in prompt
    if (s_loginActive)
        SBDL::showTexture(loginPrompt, 480, 380);

    // Add User Button
    if (SBDL::mouseInRect(addUserButtonRect))
        SBDL::showTexture(addUserButton2, addUserButtonRect);
    else
        SBDL::showTexture(addUserButton1, addUserButtonRect);

    // Trash Button
    if (SBDL::mouseInRect(trashButtonRect))
        SBDL::showTexture(trashButton2, trashButtonRect);
    else
        SBDL::showTexture(trashButton1, trashButtonRect);

    // Search button
    if (SBDL::mouseInRect(searchButtonRect))
        SBDL::showTexture(searchButton2, searchButtonRect);
    else
        SBDL::showTexture(searchButton1, searchButtonRect);

    if (s_idPlayer[0] == ' ') {
        SBDL::drawRectangle(searchButtonRect, 255, 255, 255, 170);
        SBDL::drawRectangle(trashButtonRect, 255, 255, 255, 170);
        SBDL::drawRectangle(addUserButtonRect, 255, 255, 255, 170);
    }


    // Title Texture
    Texture strAddPlayer = SBDL::createFontTexture(font3, "Add Player,", 136, 184, 147);
    Texture strRemovePlayer = SBDL::createFontTexture(font3, "Remove Player,", 180, 50, 50);
    Texture strSearchPlayer = SBDL::createFontTexture(font3, "Search Player", 50, 120, 180);
    SBDL::showTexture(strAddPlayer, 100, 90);
    SBDL::showTexture(strRemovePlayer, 280, 90);
    SBDL::showTexture(strSearchPlayer, 500, 90);
    SBDL::freeTexture(strAddPlayer);
    SBDL::freeTexture(strRemovePlayer);
    SBDL::freeTexture(strSearchPlayer);
}

void ShowGameBoardTextures() {
    // Cancel Button
    SDL_Rect cancelRect = {750, 5, 40, 40};
    SDL_Rect saveRect = {700, 5, 40, 40};
    if (SBDL::mouseInRect(cancelRect)) {
        SBDL::showTexture(cancelButton2, cancelRect);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            window = MENU;
        }
    } else {
        SBDL::showTexture(cancelButton1, cancelRect);
    }

    // Save button
    if (SBDL::mouseInRect(saveRect)) {
        SBDL::showTexture(saveButton2, saveRect);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            window = MENU;
            SaveGame();
        }
    } else {
        SBDL::showTexture(saveButton1, saveRect);
    }

    // Drawing background squares
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            SBDL::drawRectangle(game.square.backgroundSquare[i][j], 219, 219, 219);
        }
    }

    // Show bomb and number texture
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.bombs[i][j] != nullptr)
                SBDL::showTexture(bombTexture, *(game.square.bombs[i][j]));
            else if (game.square.number1[i][j] != nullptr)
                SBDL::showTexture(number1, *game.square.number1[i][j]);
            else if (game.square.number2[i][j] != nullptr)
                SBDL::showTexture(number2, *game.square.number2[i][j]);
            else if (game.square.number3[i][j] != nullptr)
                SBDL::showTexture(number3, *game.square.number3[i][j]);
            else if (game.square.number4[i][j] != nullptr)
                SBDL::showTexture(number4, *game.square.number4[i][j]);
            else if (game.square.number5[i][j] != nullptr)
                SBDL::showTexture(number5, *game.square.number5[i][j]);
            else if (game.square.number6[i][j] != nullptr)
                SBDL::showTexture(number6, *game.square.number6[i][j]);
            else if (game.square.number7[i][j] != nullptr)
                SBDL::showTexture(number7, *game.square.number7[i][j]);
            else if (game.square.number8[i][j] != nullptr)
                SBDL::showTexture(number8, *game.square.number8[i][j]);
        }
    }
}

void GameOverWindow() {
    // Removing covers
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.bombs[i][j] != nullptr)
                game.square.coverSquare[i][j] = nullptr;
        }
    }

    // Removing flags
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            game.square.flag[i][j] = nullptr;
        }
    }

    ShowGameBoardTextures();
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.coverSquare[i][j] != nullptr)
                SBDL::drawRectangle(*game.square.coverSquare[i][j], 155, 155, 155);
        }
    }

    Texture strGameOver = SBDL::createFontTexture(font2, "Game Over", 235, 64, 52);
    SBDL::showTexture(strGameOver, 575, 200);
    SBDL::freeTexture(strGameOver);
}

void WinGameWindow() {
    // Removing flags
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            game.square.flag[i][j] = nullptr;
        }
    }

    ShowGameBoardTextures();

    Texture strWin = SBDL::createFontTexture(font2, "You Won!", 0, 125, 0);
    Texture strScore = SBDL::createFontTexture(font2, "You earned " +
                                                      NumToStr(POINT) + " point", 189, 162, 11);
    SBDL::showTexture(strWin, 575, 200);
    SBDL::showTexture(strScore, 575, 250);
    SBDL::freeTexture(strWin);
    SBDL::freeTexture(strScore);
}

void AddScore(Player *p_player) {
    int countPlayers = ReadNumberOfPlayers();
    int j;
    for (j = 0; j < countPlayers; ++j) {
        if ((p_player + j)->id == game.player.id)
            break;
    }
    (p_player+j)->score = NumToStr(StrToNum((p_player+j)->score) + POINT);
    // Writing on a text file
    std::ofstream text ("player.txt");

    text << countPlayers << '\n';
    for (int i = 0; i < countPlayers; ++i) {
        text << (p_player + i)->score << '\n';
        text << (p_player + i)->id << '\n';
    }

    text.close();
}

void MainWindow(Player *p_player) {
    ShowGameBoardTextures();
    // string: Number of bombs left
    Texture strBombLeft =  SBDL::createFontTexture(font1, NumToStr(game.countBombs - CountFlags()) +
                                                          " Bombs left", 0, 0, 0);
    SBDL::showTexture(strBombLeft, 630, 50);
    SBDL::freeTexture(strBombLeft);

    // Handle mouse clicks and Draw rectangles that cover numbers and bombs
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.coverSquare[i][j] != nullptr) {
                if (SBDL::mouseInRect(*game.square.coverSquare[i][j])) {
                    SBDL::drawRectangle(*game.square.coverSquare[i][j], 200, 200, 200);
                    // Open square
                    if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED) && !IsFlagOn(i, j)) {
                        int result = OpenSquare(i, j);
                        if (result == 2) {
                            window = GAME_OVER;
                        }

                        else if (CountOpenedSquares() == pow(game.countSquareInRow, 2) - game.countBombs) {
                            AddScore(p_player);
                            window = WIN;
                        }
                    }

                    // Flag
                    else if (SBDL::Mouse.clicked(SDL_BUTTON_RIGHT, 1, SDL_PRESSED)) {
                        if (IsFlagOn(i, j)) {
                            game.square.flag[i][j] = nullptr;
                        }
                        else if (CountFlags() < game.countBombs) {
                            game.square.flag[i][j] = game.square.coverSquare[i][j];
                        }
                    }
                } else {
                    SBDL::drawRectangle(*game.square.coverSquare[i][j], 155, 155, 155);
                }
            }
        }
    }

    // Show flags
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (game.square.flag[i][j] != nullptr)
                SBDL::showTexture(flag, *game.square.flag[i][j]);
        }
    }
}

void DifficultySelectWindow() {
    Difficultly difficultly;
    bool buttonClicked = false;
    static short int inputField = LEFT; // -1: Left field  +1: Right field
    static std::string s_countBombs = " ";
    static std::string s_countSquares = " ";

    SDL_Rect easyRect = {250, 180, 50, 40};
    SDL_Rect normalRect = {330, 180, 80, 40};
    SDL_Rect hardRect = {430, 180, 50, 40};
    SDL_Rect arrowRect1 = {205, 407, 20, 20};
    SDL_Rect arrowRect2 = {567, 407, 20, 20};
    SDL_Rect inputFieldRect1 = {200, 392, 100, 50};
    SDL_Rect inputFieldRect2 = {562, 392, 100, 50};
    SDL_Rect checkRect = {715, 392, 50, 50};
    SDL_Rect cancelRect = {770, 5, 40, 40};

    // Cancel Button
    if (SBDL::mouseInRect(cancelRect))
        SBDL::showTexture(cancelButton2, cancelRect);
    else
        SBDL::showTexture(cancelButton1, cancelRect);
    if (MOUSE_LEFT_CLICKED(cancelRect))
        window = MENU;

    // Easy Button
    Texture strEasy = SBDL::createFontTexture(font1, "Easy", 11, 115, 25);
    if (SBDL::mouseInRect(easyRect)) {
        SBDL::showTexture(strEasy, 250, 180);
    } else {
        SBDL::showTexture(strEasy, 250, 180);
        SBDL::drawRectangle(easyRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(easyRect)) {
        buttonClicked = true;
        difficultly = EASY;
    }
    SBDL::freeTexture(strEasy);

    // Normal Button
    Texture strNormal = SBDL::createFontTexture(font1, "Normal", 230, 150, 2);
    if (SBDL::mouseInRect(normalRect)) {
        SBDL::showTexture(strNormal, 330, 180);
    } else {
        SBDL::showTexture(strNormal, 330, 180);
        SBDL::drawRectangle(normalRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(normalRect)) {
        buttonClicked = true;
        difficultly = NORMAL;
    }
    SBDL::freeTexture(strNormal);

    // Hard Button
    Texture strHard = SBDL::createFontTexture(font1, "Hard", 255, 41, 41);
    if (SBDL::mouseInRect(hardRect)) {
        SBDL::showTexture(strHard, 430, 180);
    } else {
        SBDL::showTexture(strHard, 430, 180);
        SBDL::drawRectangle(hardRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(hardRect)) {
        buttonClicked = true;
        difficultly = HARD;
    }
    SBDL::freeTexture(strHard);

    // Check custom button
    if (SBDL::mouseInRect(checkRect))
        SBDL::showTexture(checkButton2, checkRect);
    else
        SBDL::showTexture(checkButton1, checkRect);

    if (s_countBombs[0] == ' ' || s_countSquares[0] == ' ') {
        SBDL::drawRectangle(checkRect, 255, 255, 255, 170);
    }

    else if (MOUSE_LEFT_CLICKED(checkRect)) {
        int square = StrToNum(s_countSquares);
        int bomb   = StrToNum(s_countBombs);
        if (square >= 5 && square <= 20 && bomb <= pow(square, 2)) {
            buttonClicked = true;
            difficultly = CUSTOM;
        }
    }

    // Draw rectangles and handle click
    if (SBDL::mouseInRect(inputFieldRect1))
        SBDL::drawRectangle(inputFieldRect1, 110, 255, 254);
    else
        SBDL::drawRectangle(inputFieldRect1, 199, 255, 254);
    if (MOUSE_LEFT_CLICKED(inputFieldRect1))
        inputField = LEFT;

    if (SBDL::mouseInRect(inputFieldRect2))
        SBDL::drawRectangle(inputFieldRect2, 110, 255, 254);
    else
        SBDL::drawRectangle(inputFieldRect2, 199, 255, 254);
    if (MOUSE_LEFT_CLICKED(inputFieldRect2))
        inputField = RIGHT;

    if (inputField == LEFT) {
        // Arrow
        if (SBDL::getTime() / 1000 % 2 == 0)
            SBDL::showTexture(arrow, arrowRect1);

        Keyboard(s_countBombs, true);
    } else {
        // Arrow
        if (SBDL::getTime() / 1000 % 2 == 0)
            SBDL::showTexture(arrow, arrowRect2);

        Keyboard(s_countSquares, true);
    }

    Texture strCountBombs = SBDL::createFontTexture(font1, s_countBombs, 0, 0, 0);
    SBDL::showTexture(strCountBombs, 228, 400);
    SBDL::freeTexture(strCountBombs);

    Texture strCountSquares = SBDL::createFontTexture(font1, s_countSquares, 0, 0, 0);
    SBDL::showTexture(strCountSquares, 590, 400);
    SBDL::freeTexture(strCountSquares);

    Texture strNumberBomb = SBDL::createFontTexture(font1, "Number of bomb:", 0, 0, 0);
    SBDL::showTexture(strNumberBomb, 25, 400);
    SBDL::freeTexture(strNumberBomb);

    Texture strSizeScreen = SBDL::createFontTexture(font1, "Size of game screen:", 0, 0, 0);
    SBDL::showTexture(strSizeScreen, 360, 400);
    SBDL::freeTexture(strSizeScreen);

    // Title1
    Texture strSelectLevel = SBDL::createFontTexture(font3, "Select Difficulty Level", 0, 162, 232);
    SBDL::showTexture(strSelectLevel, 70, 80);
    SBDL::freeTexture(strSelectLevel);

    // Title2
    Texture strCustom = SBDL::createFontTexture(font3, "Custom Mode", 0, 162, 232);
    SBDL::showTexture(strCustom, 70, 300);
    SBDL::freeTexture(strCustom);

    if (buttonClicked) {
        DeallocateGameMemory();

        // Hard mode
        if (difficultly == HARD) {
            game.countSquareInRow = 20;
            game.countBombs = 96;
        }

        // Normal mode
        else if (difficultly == NORMAL) {
            game.countSquareInRow = 12;
            game.countBombs = 28;
        }

        // Easy mode
        else if (difficultly == EASY) {
            game.countSquareInRow = 5;
            game.countBombs = 4;
        }

        // Custom mode
        else if (difficultly == CUSTOM) {
            game.countSquareInRow = StrToNum(s_countSquares);
            game.countBombs = StrToNum(s_countBombs);
        }

        AllocateGameMemory();
        CreateGameBoard();
        InsertBombs();
        CountOfBombsNearSquare();
        window = MAIN;
    }
}

void MenuWindow(Player *p_player, Save *&saveSlot) {
    int countPlayers = ReadNumberOfPlayers();
    const int x = 320;
    SDL_Rect newGameRect = {x, 200, 100, 40};
    SDL_Rect loadGameRect = {x, 250, 100, 40};
    SDL_Rect changeNameRect = {x, 300, 130, 40};
    SDL_Rect leaderboardRect = {x, 350, 120, 40};
    SDL_Rect logoutRect = {x, 400, 70, 40};
    SDL_Rect quitRect = {x, 450, 50, 40};

    // New game button
    Texture strNewGame = SBDL::createFontTexture(font1, "New Game", 0, 92, 21);
    if (SBDL::mouseInRect(newGameRect)) {
        SBDL::showTexture(strNewGame, x, 200);
    } else {
        SBDL::showTexture(strNewGame, x, 200);
        SBDL::drawRectangle(newGameRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(newGameRect)) {
        window = DIFFICULTY;
        return;
    }
    SBDL::freeTexture(strNewGame);

    // load game button
    Texture strLoadGame = SBDL::createFontTexture(font1, "Load Game", 181, 167, 36);
    if (SBDL::mouseInRect(loadGameRect)) {
        SBDL::showTexture(strLoadGame, x, 250);
    } else {
        SBDL::showTexture(strLoadGame, x, 250);
        SBDL::drawRectangle(loadGameRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(loadGameRect)) {
        saveSlot = ReadSaveSlots();
        window = LOAD_GAME;
        return;
    }
    SBDL::freeTexture(strLoadGame);

    // Change name Button
    Texture strChangeName = SBDL::createFontTexture(font1, "Change Name", 36, 101, 181);
    if (SBDL::mouseInRect(changeNameRect)) {
        SBDL::showTexture(strChangeName, x, 300);
    } else {
        SBDL::showTexture(strChangeName, x, 300);
        SBDL::drawRectangle(changeNameRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(changeNameRect)) {
        window = CHANGE_NAME;
    }
    SBDL::freeTexture(strChangeName);

    // Leaderboard Button
    Texture strLeaderboard = SBDL::createFontTexture(font1, "Leaderboard", 117, 0, 90);
    if (SBDL::mouseInRect(leaderboardRect)) {
        SBDL::showTexture(strLeaderboard, x, 350);
    } else {
        SBDL::showTexture(strLeaderboard, x, 350);
        SBDL::drawRectangle(leaderboardRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(leaderboardRect)) {
        // Sorting players by score
        BubbleSort(p_player, countPlayers);
        window = LEADERBOARD;
        return;
    }
    SBDL::freeTexture(strLeaderboard);

    // Logout Button
    Texture strLogout = SBDL::createFontTexture(font1, "Logout", 255, 162, 41);
    if (SBDL::mouseInRect(logoutRect)) {
        SBDL::showTexture(strLogout, x, 400);
    } else {
        SBDL::showTexture(strLogout, x, 400);
        SBDL::drawRectangle(logoutRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(logoutRect)) {
        window = LOGIN;
        return;
    }
    SBDL::freeTexture(strLogout);

    // Quit Button
    Texture strQuit = SBDL::createFontTexture(font1, "Quit", 255, 41, 41);
    if (SBDL::mouseInRect(quitRect)) {
        SBDL::showTexture(strQuit, x, 450);
    } else {
        SBDL::showTexture(strQuit, x, 450);
        SBDL::drawRectangle(quitRect, 255, 255, 255, 170);
    }
    if (MOUSE_LEFT_CLICKED(quitRect)) {
        SBDL::stop();
    }
    SBDL::freeTexture(strQuit);

    // Title
    Texture strMenu = SBDL::createFontTexture(font3, "Menu", 0, 162, 232);
    SBDL::showTexture(strMenu, 110, 80);
    SBDL::freeTexture(strMenu);
}

void LeaderboardWindow(Player *p_player) {
    int countPlayers = ReadNumberOfPlayers();
    // Cancel Button
    SDL_Rect cancelRect = {770, 5, 40, 40};
    if (SBDL::mouseInRect(cancelRect))
        SBDL::showTexture(cancelButton2, cancelRect);
    else
        SBDL::showTexture(cancelButton1, cancelRect);
    if (MOUSE_LEFT_CLICKED(cancelRect))
        window = MENU;

    int count = countPlayers >= 5 ? 5 : countPlayers;
    for (int i = 0; i < count; ++i) {
        SDL_Rect background = {210, 195 + i*50, 400, 40};
        Texture id = SBDL::createFontTexture(font1, (p_player + i)->id, 0, 0, 0);
        Texture score = SBDL::createFontTexture(font1, (p_player + i)->score, 0, 0, 0);
        SBDL::drawRectangle(background, 201 + i*12, 255, 202 + i*12);
        SBDL::showTexture(id, 250, 200 + i*50);
        SBDL::showTexture(score, 540, 200 + i*50);
        SBDL::freeTexture(id);
        SBDL::freeTexture(score);
    }

    // Title
    Texture strLeaderboard = SBDL::createFontTexture(font3, "Leaderboard", 0, 162, 232);
    SBDL::showTexture(strLeaderboard, 110, 80);
    SBDL::freeTexture(strLeaderboard);
}

void ChangeNameWindow(Player *p_player) {
    int countPlayers = ReadNumberOfPlayers();
    static std::string s_newName = " ";
    Keyboard(s_newName);

    SDL_Rect inputFieldRect = {210, 285, 400, 70};
    SDL_Rect arrowRect = {215, 310, 20, 20};
    SDL_Rect checkRect = {350, 375, 40, 40};
    SDL_Rect cancelRect = {410, 375, 40, 40};

    // Draw Rectangles
    SBDL::drawRectangle(inputFieldRect,199, 255, 254);

    // Cancel Button
    if (SBDL::mouseInRect(cancelRect))
        SBDL::showTexture(cancelButton2, cancelRect);
    else
        SBDL::showTexture(cancelButton1, cancelRect);
    if (MOUSE_LEFT_CLICKED(cancelRect))
        window = MENU;

    // Check button
    if (SBDL::mouseInRect(checkRect))
        SBDL::showTexture(checkButton2, checkRect);
    else
        SBDL::showTexture(checkButton1, checkRect);

    if (s_newName[0] == ' ') {
        SBDL::drawRectangle(checkRect, 255, 255, 255, 170);
    }

    else if (MOUSE_LEFT_CLICKED(checkRect)) {
        // Find player in players database
        for (int i = 0; i < countPlayers; ++i) {
            if (game.player.id == (p_player + i)->id) {
                (p_player + i)->id = s_newName;
                game.player.id = s_newName;
            }
        }

        // Writing on a text file
        std::ofstream text ("player.txt");

        text << countPlayers << '\n';
        for (int i = 0; i < countPlayers; ++i) {
            text << (p_player + i)->score << '\n';
            text << (p_player + i)->id << '\n';
        }

        text.close();
        window = MENU;
        s_newName = " ";
        s_newName.shrink_to_fit();
    }

    // Arrow
    if (SBDL::getTime() / 1000 % 2 == 0)
        SBDL::showTexture(arrow, arrowRect);

    // Show new name
    Texture strNewName = SBDL::createFontTexture(font1, s_newName, 0, 0, 0);
    SBDL::showTexture(strNewName, 240, 303);
    SBDL::freeTexture(strNewName);

    // Title
    Texture strChangeName = SBDL::createFontTexture(font3, "Change Your Name", 0, 162, 232);
    SBDL::showTexture(strChangeName, 110, 80);
    SBDL::freeTexture(strChangeName);
}

void LoadGameWindow(Save *&p_saveSlot) {
    static int y = 200;
    int countSaveSlot;

    // Read number of saves
    std::ifstream read;
    std::string line;
    read.open("save.txt");
    std::getline(read, line);
    read.close();
    countSaveSlot = StrToNum(line);

    // Cancel Button
    SDL_Rect cancelRect = {770, 5, 40, 40};
    if (SBDL::mouseInRect(cancelRect))
        SBDL::showTexture(cancelButton2, cancelRect);
    else
        SBDL::showTexture(cancelButton1, cancelRect);
    if (MOUSE_LEFT_CLICKED(cancelRect))
        window = MENU;

    // Down Button
    SDL_Rect downRect = {770, 300, 40, 40};
    if (SBDL::mouseInRect(downRect))
        SBDL::showTexture(down2, downRect);
    else
        SBDL::showTexture(down1, downRect);
    if (MOUSE_LEFT_CLICKED(downRect))
        y -= 21;

    // Up Button
    SDL_Rect upRect = {770, 250, 40, 40};
    if (SBDL::mouseInRect(upRect))
        SBDL::showTexture(up2, upRect);
    else
        SBDL::showTexture(up1, upRect);
    if (MOUSE_LEFT_CLICKED(upRect))
        y += y == 200 ? 0 : 21;

    // Show save slots
    for (int i = 0, j = 0; i < countSaveSlot; ++i) {
        if ((p_saveSlot + i)->id == game.player.id) {
            // Draw rectangle
            SDL_Rect background = {150, y-5 + j*50, 500, 40};
            SBDL::drawRectangle(background, 201, 255, 202);

            // Show time of save
            Texture strTime = SBDL::createFontTexture(font1, (p_saveSlot + i)->time, 0, 0, 0);
            SBDL::showTexture(strTime, 190, y + j * 50);
            SBDL::freeTexture(strTime);

            // Load button
            SDL_Rect loadThisRect = {470, y + j*50, 100, 30};
            Texture strLoadThis1 = SBDL::createFontTexture(font1, "Load This", 114, 224, 129);
            Texture strLoadThis2 = SBDL::createFontTexture(font1, "Load This", 14, 158, 33);
            if (SBDL::mouseInRect(loadThisRect)) {
                SBDL::showTexture(strLoadThis2, 470, y + j * 50);
            } else {
                SBDL::showTexture(strLoadThis1, 470, y + j * 50);
            }
            if (MOUSE_LEFT_CLICKED(loadThisRect)) {
                window = MAIN;
                LoadGame(p_saveSlot + i);
                delete[] p_saveSlot;
                return;
            }
            SBDL::freeTexture(strLoadThis1);
            SBDL::freeTexture(strLoadThis2);

            // Trash button
            SDL_Rect trashRect = {590, y+3 + j*50, 25, 25};
            if (SBDL::mouseInRect(trashRect)) {
                SBDL::showTexture(trashButton2, trashRect);
            } else {
                SBDL::showTexture(trashButton1, trashRect);
            }
            if (MOUSE_LEFT_CLICKED(trashRect)) {
                DeleteSaveSlot(p_saveSlot, i);
                return;
            }

            ++j;
        }
    }

    if (countSaveSlot == 0) {
        Texture strNotFound = SBDL::createFontTexture(font1, "Not Found", 255, 33, 33);
        SBDL::showTexture(strNotFound, 370, 200);
        SBDL::freeTexture(strNotFound);
    }

    // Draw white rectangle
    SDL_Rect coverRect = {0, 0, 700, 195};
    SBDL::drawRectangle(coverRect, 255, 255, 255);

    // Title
    Texture strLeaderboard = SBDL::createFontTexture(font3, "Load Game", 0, 162, 232);
    SBDL::showTexture(strLeaderboard, 110, 80);
    SBDL::freeTexture(strLeaderboard);
}

void InitializeGame() {
    const int WINDOW_WIDTH = 820;
    const int WINDOW_HEIGHT = 570;
    SBDL::InitEngine("Minesweeper", WINDOW_WIDTH, WINDOW_HEIGHT);
    LoadTextures();
    LoadFonts();
    window = LOGIN;
    srand(time(nullptr));
}

int main() {
    Player *p_players = ReadPlayers();
    Save* p_saveSlot = nullptr;

    // Preparing game window
    const int FPS = 60; //frame per second
    const int DELAY = 1000 / FPS; //delay we need at each frame
    InitializeGame();

    while (SBDL::isRunning()) {
        unsigned int startTime = SBDL::getTime();

        SBDL::updateEvents();
        SBDL::clearRenderScreen();

        //Game logic code
        if (window == LOGIN) {
            LoginWindow(p_players);
        }

        else if (window == MAIN) {
            MainWindow(p_players);
        }

        else if (window == DIFFICULTY) {
            DifficultySelectWindow();
        }

        else if (window == MENU) {
            MenuWindow(p_players, p_saveSlot);
        }

        else if (window == LEADERBOARD) {
            LeaderboardWindow(p_players);
        }

        else if (window == CHANGE_NAME) {
            ChangeNameWindow(p_players);
        }

        else if (window == LOAD_GAME) {
            LoadGameWindow(p_saveSlot);
        }

        else if (window == GAME_OVER) {
            GameOverWindow();
        }

        else if (window == WIN) {
            WinGameWindow();
        }

        SBDL::updateRenderScreen();

        unsigned int elapsedTime = SBDL::getTime() - startTime;
        if (elapsedTime < DELAY)
            SBDL::delay(DELAY - elapsedTime);
    }
}