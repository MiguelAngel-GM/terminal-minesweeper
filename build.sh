#!/bin/bash

gcc src/main.c src/game.c src/UI.c src/utils.c -o minesweeper -lncurses -Wall