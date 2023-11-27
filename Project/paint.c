#include <ncurses.h>

#include "define.h"
#include "headers.h"

void paint_BOARD(WINDOW* window, struct GAME_T* GAME, int pad) {
    box(window, 0, 0);
    int i, mv;
    for (i = 0, mv = 0; i < 12; i++) {
        if (i == 6) {
            paint_BAR((3 * i) + (pad * 2), pad, window, GAME->plansza.bar);
            mv += 5;
        }
        paint_POLE_TOP(3 * i + mv, pad, (i % 2 ? YELLOW : RED), window,
                       GAME->plansza.pole[12 - i]);
        paint_POLE_BOT(3 * i + mv, pad, (i % 2 ? RED : YELLOW), window,
                       GAME->plansza.pole[13 + i]);
    }
    paint_DWOR(3 * i + (pad * 2) + mv, pad, window, GAME->plansza.dwor);

    atrr(A_BOLD, mvwprintw(window, 0, 1, "Backgammon─1.0"));
    wrefresh(window);
}

void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole) {
    int i, y = 0 + pad, tmp = pole.liczba;
    x += pad * 2;
    mvwprintw(window, y - 1, x - (pole.number > 9 ? 1 : 0), "%d", pole.number);
    for (i = 0; i < 5; i++) {
        if (tmp-- > 0) {
            atrrCLR(pole.kolor, mvwprintw(window, y + i, x, "%s", PIONEK));
        } else {
            atrrCLR(kolor, mvwprintw(window, y + i, x, "%s", POLE_DOWN));
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
            atrrCLR(pole.kolor, mvwprintw(window, y + i, x, "%s", PIONEK));
        } else {
            atrrCLR(kolor, mvwprintw(window, y + i, x, "%s", POLE_UP));
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
        atrrCLR(MAGENTA, mvwprintw(window, y + BOARD_PIONKI_HEIGHT - i, x + 1,
                                   "%s", PIONEK));
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
        atrrCLR(CYAN, mvwprintw(window,
                                y + BOARD_HEIGHT - BOARD_PIONKI_HEIGHT - 1 + i,
                                x + 1, "%s", PIONEK));
    }
}
void paint_BAR(int x, int y, WINDOW* window, struct BAR_T bar) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        mvwprintw(window, y + i, x, "%s %s", LINE, LINE);
    }
    paint_BAR_A(x, y, window, bar);
    paint_BAR_B(x, y, window, bar);
}

void paint_DWOR(int x, int y, WINDOW* window, struct DWOR_T dwor) {
    int i, j, tmp;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        mvwprintw(window, y + i, x, "%s   %s", LINE, LINE);
    }
    for (i = 0, j = 0; i < dwor.gracz_A; i++, i % 3 == 0 ? j++ : j) {
        atrrCLR(CYAN, mvwprintw(window, y + j, x + i % 3 + 1, "%s", PIONEK));
    }
    for (i = 0, j = 0; i < dwor.gracz_B; i++, i % 3 == 0 ? j++ : j) {
        atrrCLR(MAGENTA, mvwprintw(window, y + BOARD_HEIGHT - j - 1,
                                   x + i % 3 + 1, "%s", PIONEK));
    }
}