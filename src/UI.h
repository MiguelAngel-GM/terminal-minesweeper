#ifndef UI_H
#define UI_H

#include <ncurses.h>

void mainloop();

void displayMenu();

void displayBoard(const char *board_str, const int board_rows, const int board_cols);

void updateBoard();

#endif