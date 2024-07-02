#include "UI.h"
#include "game.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void mainloop() {
    enum GameState game_state = MENU;
    uint8_t quit = 0, started = 0;
    Board *board = NULL;
    int board_rows = 0, board_cols = 0, n_mines = 0;
    char *board_str = NULL;

    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    cbreak();
	noecho();
    display_menu();
    
    while(!quit) {
        int option = getch();
        
        if(game_state == MENU) {    
            if(option == '1' || option == '2' || option == '3') {
                if(option == '1') {
                    board_rows = 8;
                    board_cols = 8;
                    n_mines = 10;
                }
                else if(option == '2') {
                    board_rows = 16;
                    board_cols = 16;
                    n_mines = 40;
                }
                else {
                    board_rows = 16;
                    board_cols = 30;
                    n_mines = 99;
                }

                board = createBoard(board_rows, board_cols);
                game_state = PLAYING;

                board_str = (char*)malloc(board_rows * board_cols * sizeof(char));
                board_to_string(board, board_str);
                display_board(board_str, board_rows, board_cols);
            }
            else if(tolower(option) == 'q') {
                quit = 1;
            }
        }
        else {
            MEVENT mouse_event;

            if(option == KEY_MOUSE) {
                if(getmouse(&mouse_event) == OK) {
                    if(mouse_event.bstate & BUTTON1_CLICKED) {
                        if(started) {

                        }
                        else {
                            started = 1;
                        }
                    }
                    else if(mouse_event.bstate & BUTTON2_CLICKED) {
                        // place flag
                    }
                }
            }
            else if(tolower(option) == 'q') {
                started = 0;
                game_state = MENU;
                display_menu();
            }
        }
    }

    if(board != NULL) {
        destroyBoard(board);
    }
    if (board_str != NULL) {
        free(board_str);
    }
}

void display_menu() {
    char *text[] = {
        "MINESWEEPER",
        "(1) Easy mode",
        "(2) Medium mode",
        "(3) Hard mode",
        "(q) Quit"
    };
    
    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    clear();

    mvprintw(2, win_cols / 2 - strlen(text[0]) / 2, "%s", text[0]);
    mvprintw(4, win_cols / 2 - strlen(text[1]) / 2, "%s", text[1]);
    mvprintw(5, win_cols / 2 - strlen(text[2]) / 2, "%s", text[2]);
    mvprintw(6, win_cols / 2 - strlen(text[3]) / 2, "%s", text[3]);
    mvprintw(win_rows - 1, 0, "%s", text[4]);

    refresh();
}

void display_board(const char *board_str, const int board_rows, const int board_cols) {
    clear();

    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    int cursor_x = win_cols / 2 - board_cols / 2;
    int cursor_y = win_rows / 2 - board_rows / 2;

    for(int i = 0; i < board_rows * board_cols; i++) {
        mvprintw(cursor_y, cursor_x, "%c", board_str[i]);

        if((i+1) % board_cols == 0) {
            cursor_y++;
            cursor_x = win_cols / 2 - board_cols / 2;
        }
        else {
            cursor_x++;
        }
    }

    refresh();
}