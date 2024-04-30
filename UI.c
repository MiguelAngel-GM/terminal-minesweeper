#include "UI.h"
#include "game.h"
#include <ctype.h>
#include <string.h>

void mainloop() {
    enum GameState game_state = MENU;
    uint8_t quit = 0;
    Tile **board = NULL;
    int board_rows = 0, board_cols = 0, n_mines = 0;

    cbreak();
	noecho();
    display_menu();
    
    while(!quit) {
        if(game_state == MENU) {
            char option = getch();
            
            switch(tolower(option)) {
                case '1':
                    board_rows = 8;
                    board_cols = 8;
                    n_mines = 10;
                    board = createBoard(board_rows, board_cols);
                    populateBoard(board, board_rows, board_cols, n_mines);
                    game_state = PLAYING;
                    display_board();
                    break;
                case '2':
                    board_rows = 16;
                    board_cols = 16;
                    n_mines = 40;
                    board = createBoard(board_rows, board_cols);
                    populateBoard(board, board_rows, board_cols, n_mines);
                    game_state = PLAYING;
                    display_board();
                    break;
                case '3':
                    board_rows = 16;
                    board_cols = 30;
                    n_mines = 99;
                    board = createBoard(board_rows, board_cols);
                    populateBoard(board, board_rows, board_cols, n_mines);
                    game_state = PLAYING;
                    display_board();
                    break;
                case 'q':
                    quit = 1;
                    break;
                default:
                    ;
            }
        }
        else {
            
        }
    }

    if(board != NULL) {
        destroyBoard(board, board_rows);
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

void display_board() {
    clear();
    
    // TODO

    refresh();
}