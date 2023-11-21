#include <ncurses.h>

#include "define.h"
#include "fun.h"

#define BOARD_WIDTH 24

void paint_POLE(int x, int y, int kolor, char* znak, WINDOW* window) {
    int i;
    wattron(window, COLOR_PAIR(kolor));
    for (i = 0; i < 5; i++) {
        mvwprintw(window, y + i, x, "%s", znak);
    }
    wattroff(window, COLOR_PAIR(kolor));
}
void paint_BAR(int x, int y, WINDOW* window) {
    int i;
    for (i = 0; i < 11; i++) {
        mvwprintw(window, y + i, x, " %s  %s", LINE, LINE);
    }
}
void paint_DWOR(int x, int y, WINDOW* window) {
    int i;
    for (i = 0; i < 11; i++) {
        mvwprintw(window, y + i, x, " %s   %s", LINE, LINE);
    }
}

void paint_BOARD(WINDOW* window) {
    int i;
    for (i = 0; i < 14; i++) {
        if (i == 6) {
            paint_BAR(1 + 3 * i++, 1, window);
            continue;
        }
        paint_POLE(3 * i + 2, 1, (i % 2 ? RED : YELLOW), POLE_DOWN, window);
        paint_POLE(3 * i + 2, 7, (i % 2 ? YELLOW : RED), POLE_UP, window);
    }
    paint_DWOR(3 * i + 1, 1, window);
}

void run() {
    // WINDOW* BOARD = newwin(13, 50, 1, 4);
    WINDOW* BOARD = newwin(13, 50, 0, 0);
    refresh();
    box(BOARD, 0, 0);
    paint_BOARD(BOARD);
    wrefresh(BOARD);

    refresh();
}

int main() {
    if (init()) {
        run();
    }
    getch();
    endwin();
    return 0;
}
