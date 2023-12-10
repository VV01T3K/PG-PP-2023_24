#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "text.h"
// fun.c
void colorInit();
char initialInit();
void initWindows(GAME_T* GAME);

// paint.c
void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    POLE_T pole);
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    POLE_T pole);
void paint_BAR_A(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR_B(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_DWOR(int x, int y, WINDOW* window, DWOR_T dwor);
void paint_BOARD(WINDOW* window, GAME_T* GAME, int pad);