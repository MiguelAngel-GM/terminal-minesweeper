#include "UI.h"
#include "game.h"
#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void mainloop() {
    enum GameState game_state = MENU;
    uint8_t quit = 0, started = 0;
    Board *board = NULL;
    int n_mines = 0;
    char *board_str = NULL;

    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    curs_set(0);
    cbreak();
	noecho();
    displayMenu();
    
    while(!quit) {
        int option = getch();
        
        if(game_state == MENU) {    
            if(option == '1' || option == '2' || option == '3') {
                if(board != NULL)
                    destroyBoard(board);

                if(option == '1') {
                    board = createBoard(8, 8);
                    n_mines = 10;
                }
                else if(option == '2') {
                    board = createBoard(16, 16);
                    n_mines = 40;
                }
                else {
                    board = createBoard(16, 30);
                    n_mines = 99;
                }

                game_state = PLAYING;

                if(board_str != NULL)
                    free(board_str);
                board_str = (char*)malloc(board->n_rows * board->n_cols * sizeof(char));
                boardToString(board, board_str);
                displayBoard(board_str, board->n_rows, board->n_cols);
            }
            else if(tolower(option) == 'q') {
                quit = 1;
            }
        }
        else if(game_state == PLAYING) {
            MEVENT mouse_event;

            if(option == KEY_MOUSE) {
                if(getmouse(&mouse_event) == OK) {
                    int row, col;
                    
                    if(clickToBoardCoordinates(board, mouse_event.x, mouse_event.y, &row, &col)) {
                        if(mouse_event.bstate & BUTTON1_CLICKED) {
                            if(!started) {
                                populateBoard(board, n_mines, row, col);
                                started = 1;
                            }
                            
                            if(discoverTile(board, row, col)) {
                                boardToString(board, board_str);
                                displayBoard(board_str, board->n_rows, board->n_cols);
                                if(board->n_discovered == board->n_rows * board->n_cols - n_mines) {
                                    started = 0;
                                    game_state = GAME_ENDED;
                                }    
                            }
                            else {
                                started = 0;
                                game_state = GAME_ENDED;
                            }
                            
                        }
                        else if(mouse_event.bstate & BUTTON3_CLICKED) {
                            placeFlag(board, row, col);
                            boardToString(board, board_str);
                            displayBoard(board_str, board->n_rows, board->n_cols);
                        }
                    }
                }
            }
            else if(tolower(option) == 'm') {
                started = 0;
                game_state = MENU;
                displayMenu();
            }
        }
        else {
            clear();
            refresh();
            if(tolower(option) == 'r') {
                int n_cols = board->n_cols;
                int n_rows = board->n_rows;

                destroyBoard(board);
                board = createBoard(n_rows, n_cols);
                boardToString(board, board_str);
                displayBoard(board_str, board->n_rows, board->n_cols);

                game_state = PLAYING;
            }
            else if(tolower(option) == 'm') {
                game_state = MENU;
                displayMenu();
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

void displayMenu() {
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

void displayBoard(const char *board_str, const int board_rows, const int board_cols) {
    clear();

    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    int cursor_x = win_cols / 2 - board_cols / 2;
    int cursor_y = win_rows / 2 - board_rows / 2;

    for(int i = 0; i < board_cols; i++) {
        mvprintw(cursor_y - 1, cursor_x + i, "-");
        mvprintw(cursor_y + board_rows, cursor_x + i, "-");
    }

    for(int i = 0; i < board_rows * board_cols; i++) {
        mvprintw(cursor_y, cursor_x, "%c", board_str[i]);

        if((i+1) % board_cols == 0) {
            mvprintw(cursor_y, cursor_x - board_cols, "|");
            mvprintw(cursor_y, cursor_x + 1, "|");
            cursor_y++;
            cursor_x = win_cols / 2 - board_cols / 2;
        }
        else {
            cursor_x++;
        }
    }

    mvprintw(win_rows - 1, 0, "(m) Menu");

    refresh();
}