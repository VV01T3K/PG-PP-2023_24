#include "fun.h"

#include <locale.h>
#include <ncurses.h>

#include "define.h"

void colorInit() {
    //
    init_pair(1, COLOR_BLUE, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
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