#ifndef UI_H
#define UI_H

#include <ncurses.h>

// Some custom colors apart from the 7 included in ncurses
#define COLOR_GRAY 8
#define COLOR_ORANGE 9
#define COLOR_PINK 10

enum GameState {
    MENU,
    PLAYING,
	GAME_ENDED
};

void mainloop();

void displayMenu();

void displayBoard(const char *board_str, const int board_rows, const int board_cols);

void displayEndMessage(uint8_t player_wins);

#endif