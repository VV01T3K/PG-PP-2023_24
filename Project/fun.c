#include "fun.h"

#include <locale.h>
#include <ncurses.h>

#include "define.h"

void colorInit() {
    init_pair(1, COLOR_BLUE, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_color(COLOR_BROWN, 500, 250, 0);
    init_color(COLOR_DARK_BLUE, 0, 0, 500);
    init_pair(6, COLOR_BROWN, COLOR_BLACK);
    init_pair(7, COLOR_DARK_BLUE, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(9, COLOR_CYAN, COLOR_BLACK);
}

char init() {
    setlocale(LC_ALL, "");
    initscr();
    if (has_colors() == TRUE) {
        start_color();
        colorInit();
    } else {
        printw("Terminal nie obsługuje kolorów!");
        return FALSE;
    }
    return TRUE;
}