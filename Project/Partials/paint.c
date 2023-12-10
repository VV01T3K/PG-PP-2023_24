#include "headers.h"
void paint_MENU(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "Menu"););

    watrr(A_BOLD, w_mvwprintw(2, getmaxy(window) / 1.2, TXT_GAME_NAME););

    w_mvwprintw(4, 3, "Choose from list: ");
    w_mvwprintw(5, 3, "( Enter x2 to confirm :v )");

    w_mvwprintw(8, 4, "1) New Game");
    w_mvwprintw(9, 4, "2) Load Game");
    w_mvwprintw(10, 4, "3) Replay");
    w_mvwprintw(11, 4, "4) Exit");

    wrefresh(window);
}
void paint_NAME(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(1, 1, TXT_NAME););
    wrefresh(window);
}

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

void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window, POLE_T pole) {
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
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window, POLE_T pole) {
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
void paint_CONTROLS(WINDOW* window, GAME_T* GAME, int gracz) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(getmaxy(window) - 1, 1, "Controls"););

    wrefresh(window);
}

void printDICE(WINDOW* win, int y, int x, GAME_T* GAME, int index) {
    if (GAME->dice[index] == 0) {
        mvwprintw(win, y, x, "%d-|#|", index + 1);
    } else if (GAME->dice[index] != -1) {
        mvwprintw(win, y, x, "%d-|%d|", index + 1, GAME->dice[index]);
    }
}
void paint_DICE(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(getmaxy(window) - 1, 1, "Dices"););

    if (GAME->dice[0] == -1) {
        w_mvwprintw(2, 6, TXT_DICE_ROLL);
        wrefresh(window);
        return;
    }

    printDICE(window, 2, 6, GAME, 0);
    printDICE(window, 2, 16, GAME, 1);
    printDICE(window, 3, 6, GAME, 2);
    printDICE(window, 3, 16, GAME, 3);

    if (GAME->dublet) {
        w_mvwprintw(4, 6, "Dublet!");
    }

    wrefresh(window);
}
void paint_FAME(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(1, 7, "Hall of Fame"););
    FILE* file = fopen(FAME_PATH, "r");
    char buffer[MAX_NAME];
    for (int i = 0; i < MAX_FAME; i++) {
        fscanf(file, "%s %d", buffer, &GAME->fame.gracz[i].wynik);
        if (buffer[0] == '\0') break;
        strcpy(GAME->fame.gracz[i].nazwa, buffer);
        buffer[0] = '\0';
        mvwprintw(window, i + 3, 2, "%d) %s %d", i + 1,
                  GAME->fame.gracz[i].nazwa, GAME->fame.gracz[i].wynik);
    }
    fclose(file);

    wrefresh(window);
}
void paint_STATE(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "State"););

    watrr(A_BOLD, w_mvwprintw(2, 10, "SCORE"););
    mvwprintw(window, 3, 6, "A: %d  |  B: %d", GAME->gracz_A.wynik,
              GAME->gracz_B.wynik);
    wmove(window, 4, 4);
    for (int i = 0; i < 18; i++) w_wprintw(HR);

    watrr(A_BOLD, w_mvwprintw(5, 7, "TURN:"););
    mvwprintw(window, 5, 13, "Nr %d", GAME->turn);

    watrr(A_BOLD, w_mvwprintw(6, 5, "MOVES LEFT:"););
    mvwprintw(window, 6, 18, "%d", GAME->leftMoves);

    wmove(window, 7, 4);
    for (int i = 0; i < 18; i++) w_wprintw(HR);
    wrefresh(window);
    paint_DICE(GAME->ui_2.window, GAME);
}
void paint_GAMEVIEW(GAME_T* GAME) {
    paint_STATE(GAME->aside.window, GAME);
    paint_DICE(GAME->ui_2.window, GAME);
    paint_CONTROLS(GAME->controls.window, GAME, PLAYER_A);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
}
