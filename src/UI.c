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
    char *board_str = NULL;

    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    curs_set(0);
    cbreak();
	noecho();
    
    start_color();
    init_color(COLOR_GRAY, 200, 200, 200);
    init_color(COLOR_ORANGE, 1000, 400, 0);
    init_color(COLOR_PINK, 1000, 0, 1000);

    displayMenu();
    
    while(!quit) {
        int option = getch();
        
        if(game_state == MENU) {    
            if(option == '1' || option == '2' || option == '3') {
                if(board != NULL)
                    destroyBoard(board);

                if(option == '1') {
                    board = createBoard(8, 8, 10);
                }
                else if(option == '2') {
                    board = createBoard(16, 16, 40);
                }
                else {
                    board = createBoard(16, 30, 99);
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
                                populateBoard(board, row, col);
                                started = 1;
                            }
                            
                            if(discoverTile(board, row, col)) {
                                boardToString(board, board_str);
                                displayBoard(board_str, board->n_rows, board->n_cols);
                                if(allClear(board)) {
                                    started = 0;
                                    game_state = GAME_ENDED;
                                    displayEndMessage(1);
                                }    
                            }
                            else {
                                started = 0;
                                game_state = GAME_ENDED;
                                discoverAllMines(board);
                                boardToString(board, board_str);
                                displayBoard(board_str, board->n_rows, board->n_cols);
                                displayEndMessage(0);
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
            if(tolower(option) == 'r') {
                int n_cols = board->n_cols;
                int n_rows = board->n_rows;
                int n_mines = board->n_mines;

                destroyBoard(board);
                board = createBoard(n_rows, n_cols, n_mines);
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

    static int number_colors[] = {
        COLOR_BLUE,
        COLOR_CYAN,     
        COLOR_GREEN,   
        COLOR_YELLOW,   
        COLOR_ORANGE,
        COLOR_RED,   
        COLOR_MAGENTA, 
        COLOR_PINK,
    };


    for(int i = 0; i < 8; i++) {
        init_pair(i + 1, number_colors[i], COLOR_GRAY); // color pairs for numbers
    }

    init_pair(9, COLOR_WHITE, COLOR_RED);   // color pair for flagged tiles
    init_pair(10, COLOR_WHITE, COLOR_GRAY);  // default color pair

    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    int cursor_x = win_cols / 2 - board_cols / 2;
    int cursor_y = win_rows / 2 - board_rows / 2;

    for(int i = 0; i < board_rows * board_cols; i++) {
        int color_pair = 10;
        if(isdigit(board_str[i])) {
            color_pair = board_str[i] - '0';
        }
        else if(board_str[i] == 'F') {
            color_pair = 9;
        }

        attron(COLOR_PAIR(color_pair));
        mvprintw(cursor_y, cursor_x, "%c", board_str[i]);
        attroff(COLOR_PAIR(color_pair));

        if((i+1) % board_cols == 0) {
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

void displayEndMessage(uint8_t player_wins) {
    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    //clear();

    char *text[] = {
        player_wins ? "YOU WIN!" : "GAME OVER!",
        "(r) Retry"
    };

    mvprintw(2, win_cols / 2 - strlen(text[0]) / 2, "%s", text[0]);
    mvprintw(win_rows - 1, win_cols - strlen(text[1]), "%s", text[1]);

    refresh();
}