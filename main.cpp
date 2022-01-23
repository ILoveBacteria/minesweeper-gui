#include "SBDL.h"
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

struct Player {
    std::string score;
    std::string id;
};

struct Game {
    int countBombs;
    int countSquareInRow;
    Player *p_logedInPlayer;
    struct Square {
        SDL_Rect **p_bombs;
        SDL_Rect **backgroundSquare;
        SDL_Rect **coverSquare;
        SDL_Rect **flag;
        SDL_Rect **p_number0;
        SDL_Rect **p_number1;
        SDL_Rect **p_number2;
        SDL_Rect **p_number3;
        SDL_Rect **p_number4;
        SDL_Rect **p_number5;
        SDL_Rect **p_number6;
        SDL_Rect **p_number7;
        SDL_Rect **p_number8;
    }square;
}game;

enum Window {
    LOGIN, MAIN, DIFFICULTY, MENU, LEADERBOARD, CHANGE_NAME
}window;

enum Difficultly {
    EASY, NORMAL, HARD, CUSTOM
};

enum Menu {
    NEW_GAME, LOAD_GAME
};

void LoginWindow(Texture enterButton1, Texture enterButton2, Texture cursor, Texture addUserButton2,
                 Texture addUserButton3, Texture trashButton1, Texture trashButton2, Texture searchButton2,
                 Texture searchButton3, Font *font, Player *&p_player, int &countPlayers);

void MainWindow(Texture bomb, Texture number1, Texture number2, Texture number3, Texture number4, Texture number5,
                Texture number6, Texture number7, Texture number8, Texture flag, Font *font);

void BubbleSort(Player *a, int SIZE);
void Swap(Player &a, Player &b);
int OpenSquare(int squareNumber, bool checkBomb = true);
void LeaderboardWindow(Font *font, Player *p_player, int countPlayers, Texture cancel1, Texture cancel2);
bool IsFlagOn(int squareNumber);
void InsertBombs();
void MenuWindow(Font *font, Player *p_player, int countPlayers);
void
DifficultySelectWindow(Font *font, Texture cursor, Texture check1, Texture check2, Texture cancel1, Texture cancel2);
void ChangeNameWindow(Font *font, Player *p_player, int countPlayers, Texture cancel1, Texture cancel2, Texture cursor,
                      Texture check1, Texture check2);
void Keyboard(std::string &str, bool onlyNumber = false);
int StrToNum(std::string &str);
int pow(int n, int power);
std::string NumToStr(int n);
bool ContainNumber(std::string str);
void CountOfBombsNearSquare();
int CountOpenedSquares();
int CountFlags();
void GameOver();

int main()
{
    // Reading players from file
    std::ifstream text;
    std::string line;
    text.open("player.txt");

    std::getline(text, line);

    int countPlayers = StrToNum(line);

    auto *p_player = new Player[countPlayers];

    for (int i = 0; i < countPlayers; ++i) {
        std::getline(text, (p_player + i)->score);
        std::getline(text, (p_player + i)->id);
    }
    text.close();

    // Preparing game window
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 670;
    SBDL::InitEngine("Minesweeper", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Loading Texture
    Texture checkButton1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/check1.png");

    Texture checkButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/check2.png");

    Texture enterButton1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/enter1.png");

    Texture enterButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/enter2.png");

    Texture cursor = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                       "Exercise/Minesweeper/Refrence/text-editor.png");

    Texture addUserButton1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                               "Exercise/Minesweeper/Refrence/user-add1.png");

    Texture addUserButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                               "Exercise/Minesweeper/Refrence/user-add2.png");

    Texture addUserButton3 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                               "Exercise/Minesweeper/Refrence/user-add3.png");

    Texture trashButton1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/trash1.png");

    Texture trashButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                             "Exercise/Minesweeper/Refrence/trash2.png");

    Texture cancelButton1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                              "Exercise/Minesweeper/Refrence/remove1.png");

    Texture cancelButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                              "Exercise/Minesweeper/Refrence/remove2.png");

    Texture searchButton2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                              "Exercise/Minesweeper/Refrence/search2.png");

    Texture searchButton3 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                              "Exercise/Minesweeper/Refrence/search3.png");

    Texture bomb = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                              "Exercise/Minesweeper/Refrence/bomb.png");

    Texture number1 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                     "Exercise/Minesweeper/Refrence/1.png");

    Texture number2 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/2.png");

    Texture number3 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/3.png");

    Texture number4 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/4.png");

    Texture number5 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/5.png");

    Texture number6 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/6.png");

    Texture number7 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/7.png");

    Texture number8 = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/8.png");

    Texture flag = SBDL::loadTexture("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                        "Exercise/Minesweeper/Refrence/flag.png");

    Font *font = SBDL::loadFont("D:/Our Information/Documents/Univercity/Mabani Computer Class/"
                                "Exercise/Minesweeper/Refrence/comic.ttf", 20);

    // Initializing
    const int FPS = 30; //frame per second
    const int DELAY = 1000 / FPS; //delay we need at each frame
    window = MENU;
    srand(time(nullptr));

    while (SBDL::isRunning()) {
        unsigned int startTime = SBDL::getTime();

        SBDL::updateEvents();
        SBDL::clearRenderScreen();

        //Game logic code
        if (window == LOGIN) {
            LoginWindow(enterButton1, enterButton2, cursor, addUserButton2,
                        addUserButton3, trashButton1, trashButton2, searchButton2, searchButton3, font, p_player,
                        countPlayers);
        }

        else if (window == MAIN) {
            MainWindow(bomb, number1, number2, number3, number4, number5, number6, number7, number8, flag, font);
        }

        else if (window == DIFFICULTY) {
            DifficultySelectWindow(font, cursor, checkButton1, checkButton2, cancelButton1, cancelButton2);
        }

        else if (window == MENU) {
            MenuWindow(font, p_player, countPlayers);
        }

        else if (window == LEADERBOARD) {
            LeaderboardWindow(font, p_player, countPlayers, cancelButton1, cancelButton2);
        }

        SBDL::updateRenderScreen();

        unsigned int elapsedTime = SBDL::getTime() - startTime;

        if (elapsedTime < DELAY)
            SBDL::delay(DELAY - elapsedTime);

    }
}

void
LoginWindow(Texture enterButton1, Texture enterButton2, Texture cursor, Texture addUserButton2, Texture addUserButton3,
            Texture trashButton1, Texture trashButton2, Texture searchButton2, Texture searchButton3, Font *font,
            Player *&p_player, int &countPlayers) {

    SDL_Rect idFieldRect = {10, 10, 250, 70};
    SDL_Rect cursorRect = {15, 40, 15, 20};
    SDL_Rect playerList = {10, 100, 250, 150};
    SDL_Rect enterButtonRect = {300, 200, 50, 50};
    SDL_Rect addUserButtonRect = {400, 400, 50, 50};
    SDL_Rect searchButtonRect = {100, 400, 50, 50};
    SDL_Rect trashButtonRect = {250, 350, 30, 30};

    static Texture logInPrompt;

    static std::string s_idPlayer = " ";
    static bool s_logInActive = false;
    Keyboard(s_idPlayer, false);

    // Clicked on add user button
    if (SBDL::mouseInRect(addUserButtonRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
        bool idExist = false;
        for (int i = 0; i < countPlayers; ++i) {
            if (s_idPlayer == (p_player + i)->id) {
                idExist = true;
                break;
            }
        }

        if (!idExist) {
            auto *temp = new Player[countPlayers+1];

            for (int i = 0; i < countPlayers; ++i) {
                (temp + i)->score = (p_player + i)->score;
                (temp + i)->id    = (p_player + i)->id;
            }

            (temp + countPlayers)->score = "0";
            (temp + countPlayers)->id    = s_idPlayer;

            delete[] p_player;
            p_player = temp;

            // Writing on a text file
            std::ofstream text ("player.txt");

            text << ++countPlayers << '\n';

            for (int i = 0; i < countPlayers; ++i) {
                text << (p_player + i)->score << '\n';
                text << (p_player + i)->id << '\n';
            }

            text.close();

            SBDL::freeTexture(logInPrompt);
            logInPrompt = SBDL::createFontTexture(font, "Log in as: " + s_idPlayer, 255, 0, 0);

            game.p_logedInPlayer = p_player + countPlayers - 1;
            s_logInActive = true;
            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
        }
    }

    // Clicked on trash button
    else if (SBDL::mouseInRect(trashButtonRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
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
            auto *temp = new Player[countPlayers-1];

            for (int i = 0; i < countPlayers-1; ++i) {
                if (i >= idIndex) {
                    (temp + i)->score = (p_player + i + 1)-> score;
                    (temp + i)->id    = (p_player + i + 1)-> id;

                } else {
                    (temp + i)->score = (p_player + i)-> score;
                    (temp + i)->id    = (p_player + i)-> id;
                }
            }

            delete[] p_player;
            p_player = temp;

            // Writing on a text file
            std::ofstream text ("player.txt");

            text << --countPlayers << '\n';

            for (int i = 0; i < countPlayers; ++i) {
                text << (p_player + i)->score << '\n';
                text << (p_player + i)->id << '\n';
            }

            text.close();

            if (s_idPlayer == game.p_logedInPlayer->id) {
                s_logInActive = false;
                SBDL::freeTexture(logInPrompt);
                logInPrompt = SBDL::createFontTexture(font, " ", 255, 0, 0);
            }

            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
        }
    }

    // Clicked on search button
    else if (SBDL::mouseInRect(searchButtonRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
        bool idExist = false;
        int i;
        for (i = 0; i < countPlayers; ++i) {
            if (s_idPlayer == (p_player + i)->id) {
                idExist = true;
                break;
            }
        }

        if (idExist) {
            SBDL::freeTexture(logInPrompt);
            logInPrompt = SBDL::createFontTexture(font, "Log in as: " + s_idPlayer, 255, 0, 0);

            game.p_logedInPlayer = p_player + i;
            s_logInActive = true;
            s_idPlayer = " ";
            s_idPlayer.shrink_to_fit();
        }
    }

    else if (SBDL::mouseInRect(enterButtonRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED) &&
            s_logInActive) {

        window = MAIN;
        s_idPlayer = " ";
        s_idPlayer.shrink_to_fit();
    }


    // Draw Rectangles
    SBDL::drawRectangle(idFieldRect,199, 255, 254);
    SBDL::drawRectangle(playerList,199, 255, 254);

    // Cursor
    if (SBDL::getTime() / 1000 % 2 == 0)
        SBDL::showTexture(cursor, cursorRect);

    // Enter Button
    if (SBDL::mouseInRect(enterButtonRect))
        SBDL::showTexture(enterButton2, enterButtonRect);
    else
        SBDL::showTexture(enterButton1, enterButtonRect);

    if (!s_logInActive)
        SBDL::drawRectangle(enterButtonRect, 255, 255, 255, 170);

    // ID inputting string
    Texture strID = SBDL::createFontTexture(font, s_idPlayer, 0, 0, 0);
    SBDL::showTexture(strID, 400, 50);
    SBDL::freeTexture(strID);

    // Log in prompt
    SBDL::showTexture(logInPrompt, 5, 300);

    // Add User Button
    if (SBDL::mouseInRect(addUserButtonRect))
        SBDL::showTexture(addUserButton3, addUserButtonRect);
    else
        SBDL::showTexture(addUserButton2, addUserButtonRect);

    // Trash Button
    if (SBDL::mouseInRect(trashButtonRect))
        SBDL::showTexture(trashButton2, trashButtonRect);
    else
        SBDL::showTexture(trashButton1, trashButtonRect);

    // Search button
    if (SBDL::mouseInRect(searchButtonRect))
        SBDL::showTexture(searchButton3, searchButtonRect);
    else
        SBDL::showTexture(searchButton2, searchButtonRect);

    // Add Player string
    Texture strAddPlayer = SBDL::createFontTexture(font, "Add Player", 136, 184, 147);
    SBDL::showTexture(strAddPlayer, 5, 10);
    SBDL::freeTexture(strAddPlayer);
}

void MainWindow(Texture bomb, Texture number1, Texture number2, Texture number3, Texture number4, Texture number5,
                Texture number6, Texture number7, Texture number8, Texture flag, Font *font) {

    // string: Number of bombs left
    Texture strBombLeft =  SBDL::createFontTexture(font, NumToStr(game.countBombs - CountFlags()) +
                                                   " Bombs left", 0, 0, 0);
    SBDL::showTexture(strBombLeft, 630, 50);
    SBDL::freeTexture(strBombLeft);

    // Drawing background squares
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            SBDL::drawRectangle(game.square.backgroundSquare[i][j], 219, 219, 219);
        }
    }

    // Show bomb texture
    for (int i = 0; i < game.countBombs; ++i) {
        SBDL::showTexture(bomb, *(game.square.p_bombs[i]));
    }

    // Show number textures
    for (int i = 0; game.square.p_number1[i] != nullptr; ++i) {
        SBDL::showTexture(number1, *game.square.p_number1[i]);
    }

    for (int i = 0; game.square.p_number2[i] != nullptr; ++i) {
        SBDL::showTexture(number2, *game.square.p_number2[i]);
    }

    for (int i = 0; game.square.p_number3[i] != nullptr; ++i) {
        SBDL::showTexture(number3, *game.square.p_number3[i]);
    }

    for (int i = 0; game.square.p_number4[i] != nullptr; ++i) {
        SBDL::showTexture(number4, *game.square.p_number4[i]);
    }

    for (int i = 0; game.square.p_number5[i] != nullptr; ++i) {
        SBDL::showTexture(number5, *game.square.p_number5[i]);
    }

    for (int i = 0; game.square.p_number6[i] != nullptr; ++i) {
        SBDL::showTexture(number6, *game.square.p_number6[i]);
    }

    for (int i = 0; game.square.p_number7[i] != nullptr; ++i) {
        SBDL::showTexture(number7, *game.square.p_number7[i]);
    }

    for (int i = 0; game.square.p_number8[i] != nullptr; ++i) {
        SBDL::showTexture(number8, *game.square.p_number8[i]);
    }

    // Handle mouse clicks and Draw rectangles that cover numbers and bombs
    for (int i = 0; i < game.countSquareInRow * game.countSquareInRow; ++i) {
        if (game.square.coverSquare[i] != nullptr) {
            if (SBDL::mouseInRect(*game.square.coverSquare[i])) {
                SBDL::drawRectangle(*game.square.coverSquare[i], 200, 200, 200);

                if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED) && !IsFlagOn(i)) {
                    int result = OpenSquare(i);
                    if (result == 2) {
                        GameOver();
                    }

                    else if (CountOpenedSquares() == pow(game.countSquareInRow, 2) - game.countBombs) {
                        SBDL::stop();
                    }
                }

                else if (SBDL::Mouse.clicked(SDL_BUTTON_RIGHT, 1, SDL_PRESSED)) {
                    if (IsFlagOn(i)) {
                        game.square.flag[i] = nullptr;

                    }
                    else if (CountFlags() < game.countBombs) {
                        game.square.flag[i] = game.square.coverSquare[i];
                    }
                }
            }
            else
                SBDL::drawRectangle(*game.square.coverSquare[i], 155, 155, 155);
        }
    }

    // Show flags
    for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
        if (game.square.flag[i] != nullptr)
            SBDL::showTexture(flag, *game.square.flag[i]);
    }
}

void
DifficultySelectWindow(Font *font, Texture cursor, Texture check1, Texture check2, Texture cancel1, Texture cancel2) {
    Difficultly difficultly;
    bool buttonClicked = false;
    static short int inputField = -1; // -1: Left field  +1: Right field
    static std::string s_countBombs = " ";
    static std::string s_countSquares = " ";

    SDL_Rect easyRect = {300, 100, 50, 30};
    SDL_Rect normalRect = {300, 200, 80, 30};
    SDL_Rect hardRect = {300, 300, 50, 30};
    SDL_Rect cursorRect1 = {15 + (int)s_countBombs.length()*10, 40, 15, 20};
    SDL_Rect cursorRect2 = {500 + (int)s_countSquares.length()*10, 40, 15, 20};
    SDL_Rect inputFieldRect1 = {10, 40, 100, 50};
    SDL_Rect inputFieldRect2 = {500, 40, 100, 50};
    SDL_Rect checkRect = {10, 500, 50, 50};
    SDL_Rect cancelRect = {5, 5, 40, 40};

    // Cancel Button
    if (SBDL::mouseInRect(cancelRect)) {
        SBDL::showTexture(cancel2, cancelRect);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            window = MENU;
        }
    } else {
        SBDL::showTexture(cancel1, cancelRect);
    }

    // Easy Button
    Texture strEasy = SBDL::createFontTexture(font, "Easy", 14, 158, 33);
    if (SBDL::mouseInRect(easyRect)) {
        SBDL::showTexture(strEasy, 300, 100);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            difficultly = EASY;
        }
    } else {
        SBDL::showTexture(strEasy, 300, 100);
        SBDL::drawRectangle(easyRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strEasy);

    // Normal Button
    Texture strNormal = SBDL::createFontTexture(font, "Normal", 212, 179, 17);
    if (SBDL::mouseInRect(normalRect)) {
        SBDL::showTexture(strNormal, 300, 200);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            difficultly = NORMAL;
        }
    } else {
        SBDL::showTexture(strNormal, 300, 200);
        SBDL::drawRectangle(normalRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strNormal);

    // Hard Button
    Texture strHard = SBDL::createFontTexture(font, "Hard", 189, 67, 26);
    if (SBDL::mouseInRect(hardRect)) {
        SBDL::showTexture(strHard, 300, 300);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            difficultly = HARD;
        }
    } else {
        SBDL::showTexture(strHard, 300, 300);
        SBDL::drawRectangle(hardRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strHard);

    // Check custom button
    if (SBDL::mouseInRect(checkRect))
        SBDL::showTexture(check2, checkRect);
    else
        SBDL::showTexture(check1, checkRect);

    if (s_countBombs[0] == ' ' || s_countSquares[0] == ' ') {
        SBDL::drawRectangle(checkRect, 255, 255, 255, 170);
    }

    else if (SBDL::mouseInRect(checkRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
        int square = StrToNum(s_countSquares);
        int bomb   = StrToNum(s_countBombs);
        if (square >= 5 && square <= 20 && bomb <= pow(square, 2)) {
            buttonClicked = true;
            difficultly = CUSTOM;
        }
    }

    // Clicked on fields
    if (SBDL::mouseInRect(inputFieldRect1)) {
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
            inputField = -1;

        SBDL::drawRectangle(inputFieldRect1, 138, 138, 138);
    } else {
        SBDL::drawRectangle(inputFieldRect1, 170, 170, 170);
    }

    if (SBDL::mouseInRect(inputFieldRect2)) {
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
            inputField = 1;

        SBDL::drawRectangle(inputFieldRect2, 138, 138, 138);
    } else {
        SBDL::drawRectangle(inputFieldRect2, 170, 170, 170);
    }

    if (inputField == -1) {
        // Cursor
        if (SBDL::getTime() / 1000 % 2 == 0)
            SBDL::showTexture(cursor, cursorRect1);

        Keyboard(s_countBombs, true);

    } else {
        // Cursor
        if (SBDL::getTime() / 1000 % 2 == 0)
            SBDL::showTexture(cursor, cursorRect2);

        Keyboard(s_countSquares, true);
    }

    Texture strCountBombs = SBDL::createFontTexture(font, s_countBombs, 0, 0, 0);
    SBDL::showTexture(strCountBombs, 15, 40);
    SBDL::freeTexture(strCountBombs);

    Texture strCountSquares = SBDL::createFontTexture(font, s_countSquares, 0, 0, 0);
    SBDL::showTexture(strCountSquares, 500, 40);
    SBDL::freeTexture(strCountSquares);

    if (buttonClicked) {
        // Deallocating memories
        for (int i = 0; i < game.countSquareInRow; ++i)
            delete[] *game.square.backgroundSquare;
        delete[] game.square.backgroundSquare;
        delete[] game.square.coverSquare;
        delete[] game.square.flag;

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
            game.countBombs       = StrToNum(s_countBombs);
        }

        // Allocating memory
        game.square.backgroundSquare= new SDL_Rect*[game.countSquareInRow];
        for (int i = 0; i < game.countSquareInRow; ++i)
            game.square.backgroundSquare[i] = new SDL_Rect[game.countSquareInRow];
        game.square.coverSquare = new SDL_Rect*[pow(game.countSquareInRow, 2)];
        game.square.flag        = new SDL_Rect*[pow(game.countSquareInRow, 2)];

        for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
            game.square.flag[i] = nullptr;
        }

        int startx = 20, starty = 3;

        // Creating squares
        for (int i = 0, k = 0; i < game.countSquareInRow; ++i) {
            for (int j = 0; j < game.countSquareInRow; ++j, ++k) {
                game.square.backgroundSquare[i][j].x = startx;
                game.square.backgroundSquare[i][j].y = starty;
                game.square.backgroundSquare[i][j].w = 25;
                game.square.backgroundSquare[i][j].h = 25;

                // Copy square address
                game.square.coverSquare[k] = &game.square.backgroundSquare[i][j];

                startx += 28;
            }

            startx = 20;
            starty += 28;
        }

        InsertBombs();
        CountOfBombsNearSquare();
        window = MAIN;
    }
}

void MenuWindow(Font *font, Player *p_player, int countPlayers) {
    Menu menu;
    bool buttonClicked = false;

    SDL_Rect newGameRect     = {300, 100, 100, 30};
    SDL_Rect loadGameRect    = {300, 200, 100, 30};
    SDL_Rect changeNameRect  = {300, 300, 130, 30};
    SDL_Rect leaderboardRect = {300, 400, 120, 30};
    SDL_Rect quitRect        = {300, 500, 50, 30};

    // New game button
    Texture strNewGame = SBDL::createFontTexture(font, "New Game", 14, 158, 33);
    if (SBDL::mouseInRect(newGameRect)) {
        SBDL::showTexture(strNewGame, 300, 100);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            menu = NEW_GAME;
        }
    } else {
        SBDL::showTexture(strNewGame, 300, 100);
        SBDL::drawRectangle(newGameRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strNewGame);

    // load game button
    Texture strLoadGame = SBDL::createFontTexture(font, "Load Game", 212, 179, 17);
    if (SBDL::mouseInRect(loadGameRect)) {
        SBDL::showTexture(strLoadGame, 300, 200);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            menu = LOAD_GAME;
        }
    } else {
        SBDL::showTexture(strLoadGame, 300, 200);
        SBDL::drawRectangle(loadGameRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strLoadGame);

    // Change name Button
    Texture strChangeName = SBDL::createFontTexture(font, "Change Name", 189, 67, 26);
    if (SBDL::mouseInRect(changeNameRect)) {
        SBDL::showTexture(strChangeName, 300, 300);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            buttonClicked = true;
            window = CHANGE_NAME;
        }
    } else {
        SBDL::showTexture(strChangeName, 300, 300);
        SBDL::drawRectangle(changeNameRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strChangeName);

    // Leaderboard Button
    Texture strLeaderboard = SBDL::createFontTexture(font, "Leaderboard", 189, 67, 26);
    if (SBDL::mouseInRect(leaderboardRect)) {
        SBDL::showTexture(strLeaderboard, 300, 400);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            // Sorting players by score
            BubbleSort(p_player, countPlayers);
            window = LEADERBOARD;
            return;
        }
    } else {
        SBDL::showTexture(strLeaderboard, 300, 400);
        SBDL::drawRectangle(leaderboardRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strLeaderboard);

    // Quit Button
    Texture strQuit = SBDL::createFontTexture(font, "Quit", 189, 67, 26);
    if (SBDL::mouseInRect(quitRect)) {
        SBDL::showTexture(strQuit, 300, 500);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            SBDL::stop();
        }
    } else {
        SBDL::showTexture(strQuit, 300, 500);
        SBDL::drawRectangle(quitRect, 255, 255, 255, 170);
    }
    SBDL::freeTexture(strQuit);

    if (buttonClicked) {
        if (menu == NEW_GAME) {
            window = DIFFICULTY;
        }
    }
}

void LeaderboardWindow(Font *font, Player *p_player, int countPlayers, Texture cancel1, Texture cancel2) {
    SDL_Rect cancelRect = {5, 5, 40, 40};

    int count = countPlayers >= 5 ? 5 : countPlayers;
    for (int i = 0; i < count; ++i) {
        Texture id = SBDL::createFontTexture(font, (p_player+i)->id, 0, 0, 0);
        Texture score = SBDL::createFontTexture(font, (p_player+i)->score, 0, 0, 0);
        SBDL::showTexture(id, 10, 50 + i*100);
        SBDL::showTexture(score, 300, 50 + i*100);
        SBDL::freeTexture(id);
        SBDL::freeTexture(score);
    }

    if (SBDL::mouseInRect(cancelRect)) {
        SBDL::showTexture(cancel2, cancelRect);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            window = MENU;
        }
    } else {
        SBDL::showTexture(cancel1, cancelRect);
    }
}

void ChangeNameWindow(Font *font, Player *p_player, int countPlayers, Texture cancel1, Texture cancel2, Texture cursor,
                      Texture check1, Texture check2) {

    static std::string s_newName = " ";
    Keyboard(s_newName);

    SDL_Rect cursorRect = {15 + (int)s_newName.length() * 10, 40, 15, 20};
    SDL_Rect cancelRect = {5, 5, 40, 40};
    SDL_Rect checkRect = {10, 500, 50, 50};
    SDL_Rect inputFieldRect = {10, 40, 100, 50};

    SBDL::drawRectangle(inputFieldRect, 120, 120, 120);

    // Cancel button
    if (SBDL::mouseInRect(cancelRect)) {
        SBDL::showTexture(cancel2, cancelRect);
        if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
            window = MENU;
        }
    } else {
        SBDL::showTexture(cancel1, cancelRect);
    }

    // Check button
    if (SBDL::mouseInRect(checkRect))
        SBDL::showTexture(check2, checkRect);
    else
        SBDL::showTexture(check1, checkRect);

    if (s_newName[0] == ' ') {
        SBDL::drawRectangle(checkRect, 255, 255, 255, 170);
    }

    else if (SBDL::mouseInRect(checkRect) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED)) {
        // Writing on a text file
        std::ofstream text ("player.txt");

        text << countPlayers << '\n';

        for (int i = 0; i < countPlayers; ++i) {
            text << (p_player + i)->score << '\n';
            text << (p_player + i)->id << '\n';
        }

        text.close();
    }

    // Cursor
    if (SBDL::getTime() / 1000 % 2 == 0)
        SBDL::showTexture(cursor, cursorRect);

    // Show new name
    Texture strNewName = SBDL::createFontTexture(font, s_newName, 0, 0, 0);
    SBDL::showTexture(strNewName, 15, 40);
    SBDL::freeTexture(strNewName);

}

void Keyboard(std::string &str, bool onlyNumber) {
    if (SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)) {
        str.pop_back();
        if (str.length() == 0)
            str.push_back(' ');
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

bool ContainNumber(std::string str) {
    bool result = true;

    for (int i = 0; i < str.length(); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z')
            return false;
    }
    return true;
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

std::string NumToStr(int n) {
    int length = 0;
    std::string result;

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

int pow(int n, int power) {
    int result = 1;

    for (int i = 0; i < power; ++i)
        result *= n;

    return result;
}

void InsertBombs() {

    delete[] game.square.p_bombs;
    game.square.p_bombs = new SDL_Rect*[game.countBombs];

    for (int i = 0; i < game.countBombs; ++i) {
        bool continueLoop = false;
        int row = rand() % game.countSquareInRow;
        int column = rand() % game.countSquareInRow;

        // Avoiding duplicate random number
        for (int j = 0; j < i; ++j) {
            if (game.square.p_bombs[j] == &game.square.backgroundSquare[row][column]) {
                i--;
                continueLoop = true;
                break;
            }
        }

        if (continueLoop)
            continue;

        // Assigning the memory address of Rect
        game.square.p_bombs[i] = &game.square.backgroundSquare[row][column];
    }
}

void CountOfBombsNearSquare() {

    int n0 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0, n5 = 0, n6 = 0, n7 = 0, n8 = 0;
    int sum[20][20] = {0};

    game.square.p_number0 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number1 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number2 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number3 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number4 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number5 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number6 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number7 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];
    game.square.p_number8 = new SDL_Rect*[game.countSquareInRow * game.countSquareInRow - game.countBombs];

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

            for (int k = 0; k < game.countBombs; ++k) {
                // Bomb in this square?
                if (game.square.p_bombs[k] == &game.square.backgroundSquare[i][j]) {
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

                    break;
                }
            }
        }
    }

    // Setting bomb squares to 0
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            for (int k = 0; k < game.countBombs; ++k) {
                if (game.square.p_bombs[k] == &game.square.backgroundSquare[i][j])
                    sum[i][j] = -1;
            }
        }
    }

    // Assigning square address
    for (int i = 0; i < game.countSquareInRow; ++i) {
        for (int j = 0; j < game.countSquareInRow; ++j) {
            if (sum[i][j] == 0) {
                game.square.p_number0[n0] = &game.square.backgroundSquare[i][j];
                ++n0;
            }
            else if (sum[i][j] == 1) {
                game.square.p_number1[n1] = &game.square.backgroundSquare[i][j];
                ++n1;
            }

            else if (sum[i][j] == 2) {
                game.square.p_number2[n2] = &game.square.backgroundSquare[i][j];
                ++n2;
            }

            else if (sum[i][j] == 3) {
                game.square.p_number3[n3] = &game.square.backgroundSquare[i][j];
                ++n3;
            }

            else if (sum[i][j] == 4) {
                game.square.p_number4[n4] = &game.square.backgroundSquare[i][j];
                ++n4;
            }

            else if (sum[i][j] == 5) {
                game.square.p_number5[n5] = &game.square.backgroundSquare[i][j];
                ++n5;
            }

            else if (sum[i][j] == 6) {
                game.square.p_number6[n6] = &game.square.backgroundSquare[i][j];
                ++n6;
            }

            else if (sum[i][j] == 7) {
                game.square.p_number7[n7] = &game.square.backgroundSquare[i][j];
                ++n7;
            }

            else if (sum[i][j] == 8) {
                game.square.p_number8[n8] = &game.square.backgroundSquare[i][j];
                ++n8;
            }
        }
    }

    // Marking the end
    game.square.p_number0[n0] = nullptr;
    game.square.p_number1[n1] = nullptr;
    game.square.p_number2[n2] = nullptr;
    game.square.p_number3[n3] = nullptr;
    game.square.p_number4[n4] = nullptr;
    game.square.p_number5[n5] = nullptr;
    game.square.p_number6[n6] = nullptr;
    game.square.p_number7[n7] = nullptr;
    game.square.p_number8[n8] = nullptr;
}

int OpenSquare(int squareNumber, bool checkBomb) {
    /**
     * 0 = has already opened
     * 1 = exit function or opened successfully
     * 2 = bomb under the square
     */

    // avoid AIOOB
    if (game.square.coverSquare[squareNumber] == nullptr || squareNumber < 0 ||
        squareNumber >= pow(game.countSquareInRow, 2))
        return 0;

    // bomb?
    for (int i = 0; i < game.countBombs && checkBomb; ++i) {
        if (game.square.p_bombs[i] == game.square.coverSquare[squareNumber])
            return 2;
    }

    bool nearSquareIsBomb = true;
    for (int i = 0; game.square.p_number0[i] != nullptr; ++i) {
        if (game.square.p_number0[i] == game.square.coverSquare[squareNumber]) {
            nearSquareIsBomb = false;
            break;
        }
    }

    if (nearSquareIsBomb) {
        // Cover square
        game.square.coverSquare[squareNumber] = nullptr;
        // Remove flag
        game.square.flag[squareNumber] = nullptr;
        return 1;

    } else {
        // Cover square
        game.square.coverSquare[squareNumber] = nullptr;
        // Remove flag
        game.square.flag[squareNumber] = nullptr;

        // Recursively opening other empty squares
        // Special squares on the left
        if (squareNumber % game.countSquareInRow == 0) {
            OpenSquare(squareNumber - game.countSquareInRow, false);
            OpenSquare(squareNumber - game.countSquareInRow + 1, false);
            OpenSquare(squareNumber + 1, false);
            OpenSquare(squareNumber + game.countSquareInRow + 1, false);
            OpenSquare(squareNumber + game.countSquareInRow, false);
        }

        // Special squares on the right
        else if (squareNumber % game.countSquareInRow == game.countSquareInRow - 1) {
            OpenSquare(squareNumber - game.countSquareInRow - 1, false);
            OpenSquare(squareNumber - game.countSquareInRow, false);
            OpenSquare(squareNumber - 1, false);
            OpenSquare(squareNumber + game.countSquareInRow - 1, false);
            OpenSquare(squareNumber + game.countSquareInRow, false);

        // Common squares
        } else {
            OpenSquare(squareNumber - game.countSquareInRow - 1, false);
            OpenSquare(squareNumber - game.countSquareInRow, false);
            OpenSquare(squareNumber - game.countSquareInRow + 1, false);
            OpenSquare(squareNumber - 1, false);
            OpenSquare(squareNumber + 1, false);
            OpenSquare(squareNumber + game.countSquareInRow - 1, false);
            OpenSquare(squareNumber + game.countSquareInRow + 1, false);
            OpenSquare(squareNumber + game.countSquareInRow, false);
        }
    }
    return 1;
}

void GameOver() {
    // Removing covers
    for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
        for (int j = 0; j < game.countBombs; ++j) {
            if (game.square.p_bombs[j] == game.square.coverSquare[i])
                game.square.coverSquare[i] = nullptr;
        }
    }

    // Removing flags
    for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
        game.square.flag[i] = nullptr;
    }
}

int CountOpenedSquares() {
    int result = 0;
    for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
        if (game.square.coverSquare[i] == nullptr)
            ++result;
    }

    return result;
}

int CountFlags() {
    int result = 0;
    for (int i = 0; i < pow(game.countSquareInRow, 2); ++i) {
        if (game.square.flag[i] != nullptr)
            ++result;
    }

    return result;
}

bool IsFlagOn(int squareNumber) {
    if (game.square.flag[squareNumber] != nullptr)
        return true;

    return false;
}