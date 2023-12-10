#include "headers.h"

void paint_BOARD(WINDOW* window, GAME_T* GAME, int pad) {
    box(window, 0, 0);
    int i, mv;
    for (i = 0, mv = 0; i < BOARD_HEIGHT - 1; i++) {
        if (i == 6) {
            paint_BAR((3 * i) + (pad * 2), pad, window, GAME);
            mv += 5;
        }
        paint_POLE_TOP(3 * i + mv, pad, (i % 2 ? YELLOW : RED), window,
                       GAME->plansza.pole[12 - i]);
        paint_POLE_BOT(3 * i + mv, pad, (i % 2 ? RED : YELLOW), window,
                       GAME->plansza.pole[13 + i]);
    }
    paint_DWOR(3 * i + (pad * 2) + mv, pad, window, GAME->plansza.dwor);

    watrr(A_BOLD, mvwprintw(window, 0, 1, TXT_GAME_NAME));
    wrefresh(window);
}

void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    POLE_T pole) {
    int i, y = 0 + pad, tmp = pole.liczba;
    x += pad * 2;
    mvwprintw(window, y - 1, x - (pole.number > 9 ? 1 : 0), "%d", pole.number);
    for (i = 0; i < PIONKI_HEIGHT; i++) {
        if (tmp-- > 0) {
            atrrCLR(pole.kolor, mvwprintw(window, y + i, x, "%s", PIONEK));
        } else {
            atrrCLR(kolor, mvwprintw(window, y + i, x, "%s", POLE_DOWN));
        }
    }
    if (tmp > 0) mvwprintw(window, y + i, x - 1, "+%d", tmp);
}
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    POLE_T pole) {
    int i, y = 8 + pad, tmp = pole.liczba;
    x += pad * 2;
    mvwprintw(window, y + PIONKI_HEIGHT, x - 1, "%d", pole.number);
    for (i = 4; i >= 0; i--) {
        if (tmp-- > 0) {
            atrrCLR(pole.kolor, mvwprintw(window, y + i, x, "%s", PIONEK));
        } else {
            atrrCLR(kolor, mvwprintw(window, y + i, x, "%s", POLE_UP));
        }
    }
    if (tmp > 0) mvwprintw(window, y - 1, x - 1, "+%d", tmp);
}
void paint_BAR_A(int x, int y, WINDOW* window, GAME_T* GAME) {
    int i, tmp_A = BAR_PLAYER_A.liczba;
    for (i = 0; i < BAR_PLAYER_A.liczba; i++) {
        if (i >= MAX_BAR) {
            mvwprintw(window, y + PIONKI_HEIGHT - i, x, "+%d",
                      BAR_PLAYER_A.liczba - i);
            break;
        }
        atrrCLR(CLR_PLAYER_A,
                mvwprintw(window, y + PIONKI_HEIGHT - i, x + 1, "%s", PIONEK));
    }
}
void paint_BAR_B(int x, int y, WINDOW* window, GAME_T* GAME) {
    int i, tmp_A = BAR_PLAYER_B.liczba;
    for (i = 0; i < BAR_PLAYER_B.liczba; i++) {
        if (i >= MAX_BAR) {
            mvwprintw(window, y + BOARD_HEIGHT - PIONKI_HEIGHT - 1 + i, x,
                      "+%d", BAR_PLAYER_B.liczba - i);
            break;
        }
        atrrCLR(CLR_PLAYER_B,
                mvwprintw(window, y + BOARD_HEIGHT - PIONKI_HEIGHT - 1 + i,
                          x + 1, "%s", PIONEK));
    }
}
void paint_BAR(int x, int y, WINDOW* window, GAME_T* GAME) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        mvwprintw(window, y + i, x, "%s %s", LINE, LINE);
    }
    paint_BAR_A(x, y, window, GAME);
    paint_BAR_B(x, y, window, GAME);
}

void paint_DWOR(int x, int y, WINDOW* window, DWOR_T dwor) {
    int i, j, tmp;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        mvwprintw(window, y + i, x, "%s   %s", LINE, LINE);
    }
    for (i = 0, j = 0; i < dwor.gracz_B; i++, i % 3 == 0 ? j++ : j) {
        atrrCLR(CLR_PLAYER_B,
                mvwprintw(window, y + j, x + i % 3 + 1, "%s", PIONEK));
    }
    for (i = 0, j = 0; i < dwor.gracz_A; i++, i % 3 == 0 ? j++ : j) {
        atrrCLR(CLR_PLAYER_A, mvwprintw(window, y + BOARD_HEIGHT - j - 1,
                                        x + i % 3 + 1, "%s", PIONEK));
    }
}