#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "text.h"
// misc.c
void colorInit();
char initialInit();
void initWindows(GAME_T* GAME);
int asc_fame(const void* a, const void* b);
int desc_fame(const void* a, const void* b);
int asc_dice(const void* a, const void* b);
int desc_dice(const void* a, const void* b);
int asc_KOSTKA(const void* a, const void* b);

// paint.c
void paint_MENU(WINDOW* window, GAME_T* GAME);
void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window, POLE_T pole);
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window, POLE_T pole);
void paint_BAR_A(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR_B(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_DWOR(int x, int y, WINDOW* window, DWOR_T dwor);
void paint_BOARD(WINDOW* window, GAME_T* GAME, int pad);
void paint_GAMEVIEW(GAME_T* GAME);
void paint_STATE(WINDOW* window, GAME_T* GAME);
void paint_FAME(WINDOW* window, GAME_T* GAME);
void paint_CONTROLS(WINDOW* window, GAME_T* GAME, int gracz);
void printDICE(WINDOW* win, int y, int x, GAME_T* GAME, int index);
void paint_DICE(WINDOW* window, GAME_T* GAME);
void paint_NAME(WINDOW* window, GAME_T* GAME);

// index.c
void crud_move_pionek(GAME_T* GAME, int start, int cel, int gracz);
void mu_move(WINDOW* window, GAME_T* GAME, int gracz, int start);
void exec_win(GAME_T* GAME, int points);
void capture(GAME_T* GAME, int docelowe, int gracz);
void initGame(GAME_T* GAME);
void run(GAME_T* GAME);