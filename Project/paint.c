#include <ncurses.h>

#include "define.h"
#include "headers.h"

void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole) {
    int i, y = 0 + pad, tmp = pole.liczba;
    x += pad * 2;
    mvwprintw(window, y - 1, x - (pole.number > 9 ? 1 : 0), "%d", pole.number);
    for (i = 0; i < 5; i++) {
        if (tmp-- > 0) {
            wattron(window, COLOR_PAIR(pole.kolor));
            mvwprintw(window, y + i, x, "%s", PIONEK);
            wattroff(window, COLOR_PAIR(pole.kolor));
        } else {
            wattron(window, COLOR_PAIR(kolor));
            mvwprintw(window, y + i, x, "%s", POLE_DOWN);
            wattroff(window, COLOR_PAIR(kolor));
        }
    }
    if (tmp > 0) mvwprintw(window, y + i, x - 1, "+%d", tmp);
}
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole) {
    int i, y = 8 + pad, tmp = pole.liczba;
    x += pad * 2;
    mvwprintw(window, y + 5, x - 1, "%d", pole.number);
    for (i = 4; i >= 0; i--) {
        if (tmp-- > 0) {
            wattron(window, COLOR_PAIR(pole.kolor));
            mvwprintw(window, y + i, x, "%s", PIONEK);
            wattroff(window, COLOR_PAIR(pole.kolor));
        } else {
            wattron(window, COLOR_PAIR(kolor));
            mvwprintw(window, y + i, x, "%s", POLE_UP);
            wattroff(window, COLOR_PAIR(kolor));
        }
    }
    if (tmp > 0) mvwprintw(window, y - 1, x - 1, "+%d", tmp);
}
void paint_BAR_A(int x, int y, WINDOW* window, struct BAR_T bar) {
    int i, tmp_A = bar.gracz_A;
    for (i = 0; i < bar.gracz_A; i++) {
        if (i >= MAX_BAR) {
            mvwprintw(window, y + BOARD_PIONKI_HEIGHT - i, x, "+%d",
                      bar.gracz_A - i);
            break;
        }
        wattron(window, COLOR_PAIR(MAGENTA));
        mvwprintw(window, y + BOARD_PIONKI_HEIGHT - i, x + 1, "%s", PIONEK);
        wattroff(window, COLOR_PAIR(MAGENTA));
    }
}
void paint_BAR_B(int x, int y, WINDOW* window, struct BAR_T bar) {
    int i, tmp_A = bar.gracz_B;
    for (i = 0; i < bar.gracz_B; i++) {
        if (i >= MAX_BAR) {
            mvwprintw(window, y + BOARD_HEIGHT - BOARD_PIONKI_HEIGHT - 1 + i, x,
                      "+%d", bar.gracz_B - i);
            break;
        }
        wattron(window, COLOR_PAIR(CYAN));
        mvwprintw(window, y + BOARD_HEIGHT - BOARD_PIONKI_HEIGHT - 1 + i, x + 1,
                  "%s", PIONEK);
        wattroff(window, COLOR_PAIR(CYAN));
    }
}
void paint_BAR(int x, int y, WINDOW* window, struct BAR_T bar) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        mvwprintw(window, y + i, x, "%s %s", LINE, LINE);
    }
    paint_BAR_A(x, y, window, bar);
    paint_BAR_B(x, y, window, bar);
}