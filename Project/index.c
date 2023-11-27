#include <ncurses.h>

#include "define.h"
#include "fun.h"

#define BOARD_WIDTH 24

void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole) {
    int pionki = pole.liczba, y = 0 + pad;
    x += pad * 2;
    for (int i = 0; i < 5; i++) {
        if (pionki-- > 0) {
            wattron(window, COLOR_PAIR(pole.kolor));
            mvwprintw(window, y + i, x, "%s", PIONEK);
            wattroff(window, COLOR_PAIR(pole.kolor));
        } else {
            wattron(window, COLOR_PAIR(kolor));
            mvwprintw(window, y + i, x, "%s", POLE_DOWN);
            wattroff(window, COLOR_PAIR(kolor));
        }
        // mvwprintw(window, y + i, x, "%d", pole.number % 7);
    }
}

void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole) {
    int pionki = pole.liczba, y = 8 + pad;
    x += pad * 2;
    for (int i = 4; i >= 0; i--) {
        if (pionki-- > 0) {
            wattron(window, COLOR_PAIR(pole.kolor));
            mvwprintw(window, y + i, x, "%s", PIONEK);
            wattroff(window, COLOR_PAIR(pole.kolor));
        } else {
            wattron(window, COLOR_PAIR(kolor));
            mvwprintw(window, y + i, x, "%s", POLE_UP);
            wattroff(window, COLOR_PAIR(kolor));
        }
        // mvwprintw(window, y + i, x, "%d", pole.number % 7);
    }
}

void paint_BAR(int x, int y, WINDOW* window) {
    int i;
    for (i = 0; i < 13; i++) {
        mvwprintw(window, y + i, x, " %s %s", LINE, LINE);
    }
}
void paint_DWOR(int x, int y, WINDOW* window) {
    int i;
    for (i = 0; i < 13; i++) {
        mvwprintw(window, y + i, x, " %s   %s", LINE, LINE);
    }
}

void paint_BOARD(struct GAME_T* GAME, WINDOW* window, int pad) {
    refresh();
    box(window, 0, 0);
    int i, mv;
    for (i = 0, mv = 0; i < 12; i++) {
        if (i == 6) {
            paint_BAR((3 * i) + (pad * 2 - 1), pad, window);
            mv += 5;
        }
        paint_POLE_TOP(3 * i + mv, pad, (i % 2 ? YELLOW : RED), window,
                       GAME->plansza.pole[12 - i]);
        paint_POLE_BOT(3 * i + mv, pad, (i % 2 ? RED : YELLOW), window,
                       GAME->plansza.pole[13 + i]);
    }
    paint_DWOR(3 * i + mv, pad, window);

    mvwprintw(window, 0, 1, "Backgammon-1.0");
    wrefresh(window);
}

void run(struct GAME_T* GAME) {
    const int padding = 2;

    WINDOW* BOARD = newwin(13 + (padding * 2), 45 + (padding * 3), 1, 4);

    paint_BOARD(GAME, BOARD, padding);

    refresh();
}

void placePionki(struct GAME_T* GAME) {
    struct {
        int index, liczba, kolor;
    } pionki[] = {{1, 2, MAGENTA},  {6, 5, CYAN},  {8, 3, CYAN},
                  {12, 5, MAGENTA}, {13, 5, CYAN}, {17, 3, MAGENTA},
                  {19, 5, MAGENTA}, {24, 2, CYAN}};

    for (int i = 0; i < sizeof(pionki) / sizeof(pionki[0]); i++) {
        GAME->plansza.pole[pionki[i].index].liczba = pionki[i].liczba;
        GAME->plansza.pole[pionki[i].index].kolor = pionki[i].kolor;
    }
}

void initGame(struct GAME_T* GAME) {
    for (int i = 1; i < 25; i++) {
        GAME->plansza.pole[i].liczba = 0;
        GAME->plansza.pole[i].kolor = 0;
        GAME->plansza.pole[i].xyPos = 0;
        GAME->plansza.pole[i].number = i;
    }
    placePionki(GAME);

    GAME->plansza.bar.gracz_A = 0;
    GAME->plansza.bar.gracz_B = 0;
    GAME->plansza.dwor.gracz_A = 0;
    GAME->plansza.dwor.gracz_B = 0;

    GAME->gracz_A.wynik = 0;
    GAME->gracz_B.wynik = 0;

    // for (i = 0; i < MAX_HALL_OF_FAME; i++) {
    //     GAME->hall_of_fame.gracz[i].wynik = 0;
    //     strcpy(GAME->hall_of_fame.gracz[i].nazwa, "Gracz");
    // }
}

int main() {
    if (init()) {
        struct GAME_T* GAME = (struct GAME_T*)malloc(sizeof(struct GAME_T));
        if (GAME == NULL) return 1;
        initGame(GAME);
        run(GAME);
    }
    getch();
    endwin();
    return 0;
}
