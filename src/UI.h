#ifndef UI_H
#define UI_H

#include <ncurses.h>

void mainloop();

void display_menu();

void display_board(const char *board_str, const int board_rows, const int board_cols);

void update_board();

#endif