#include <ncurses.h>

#include "define.h"
#include "headers.h"

void paint_MENU(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    mvwprintw(window, 0, 1, "cos");
    wrefresh(window);
}
void paint_UI(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    mvwprintw(window, getmaxy(window) - 1, 1, "UI");
    mvwprintw(window, getmaxy(window) / 2, 2, "komendy");
    wrefresh(window);
}

void run(struct GAME_T* GAME) {
    printw("Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24");

    paint_MENU(GAME->menu.window, GAME);
    paint_UI(GAME->ui.window, GAME);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
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
    if (initialInit()) {
        struct GAME_T* GAME = (struct GAME_T*)malloc(sizeof(struct GAME_T));
        if (GAME == NULL) return 1;
        initWindows(GAME);
        initGame(GAME);
        run(GAME);
    }
    getch();
    endwin();
    return 0;
}
