#ifndef UI_H
#define UI_H

#include <ncurses.h>

enum GameState {
    MENU,
    PLAYING
};

void mainloop();

void display_menu();

void display_board();

void update_board();

#endif