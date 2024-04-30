#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "UI.h"

int main() {
	srand(time(NULL));
	
	initscr();
	mainloop();
	endwin();

	return 0;
}
