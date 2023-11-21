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

WINDOW* paint_BOARD(int pad) {
    WINDOW* window = newwin(13 + (pad * 2), 51 + (pad * 4), 0, 0);
    refresh();
    box(window, 0, 0);
    int i;
    for (i = 0; i < 14; i++) {
        if (i == 6) {
            paint_BAR(pad * 2 + 1 + 3 * i++, 1 + pad, window);
            continue;
        }
        paint_POLE(3 * i + 2 + pad * 2, 1 + pad, (i % 2 ? RED : YELLOW),
                   POLE_DOWN, window);
        paint_POLE(3 * i + 2 + pad * 2, 7 + pad, (i % 2 ? YELLOW : RED),
                   POLE_UP, window);
    }
    paint_DWOR(3 * i + +1 + pad * 2, pad + 1, window);
    // mvwprintw(window, 0, 1, "Backgammon 1.0");
    wrefresh(window);
    return window;
}

void run() {
    // WINDOW* BOARD = newwin(13, 50, 1, 4);
    const int padding = 1;
    WINDOW* BOARD = paint_BOARD(padding);

    mvwprintw(BOARD, 0, 1, "Backgammon-1.0");
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
