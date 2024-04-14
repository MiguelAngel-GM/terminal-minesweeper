#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

int main() {
	srand(time(NULL));
	
	Tile **board = createBoard(8, 8);
	populateBoard(board, 8, 8, 10);

	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			if(board[i][j].content < 255) {
				printf("%d ", board[i][j].content);
			}
			else {
				printf("* ");
			}
		}
		printf("\n");
	}

	destroyBoard(board, 8);

	return 0;
}
