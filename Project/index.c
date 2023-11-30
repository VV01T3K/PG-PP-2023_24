#include <ncurses.h>
#include <stdlib.h>

#include "define.h"
#include "headers.h"

void paint_HALL(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(1, 7, "Hall of Fame"););
    wrefresh(window);
}
void paint_CONTROLS(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    w_mvwprintw(1, 2, "Gracz A");
    w_mvwprintw(2, 2, "Ruch:");
    w_mvwprintw(getmaxy(window) - 1, 1, "Controls");
    w_mvwprintw(getmaxy(window) / 2, 4, "........................");
    wrefresh(window);
}
void paint_UI_2(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    w_mvwprintw(getmaxy(window) - 1, 1, "Kostki");
    wrefresh(window);
}
void paint_MENU(WINDOW* window, struct GAME_T* GAME) {
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "Menu"););

    watrr(A_BOLD, w_mvwprintw(2, getmaxy(window) / 1.2, "Backgammon─1.0"););

    w_mvwprintw(6, 4, "1) New Game");
    w_mvwprintw(7, 4, "2) Load Game");
    w_mvwprintw(8, 4, "3) Replay");
    w_mvwprintw(9, 4, "4) Exit");

    // w_mvwprintw( 4, 3, "Choose from the list: ");
    wrefresh(window);
}

int input(WINDOW* window, int wrong) {
    w_mvwprintw(4, 3, "Choose from list: ");
    wmove(window, 4, 3);
    wclrtoeol(window);

    wrefresh(window);
    char in = wgetch(window);
    switch (in) {
        case '1':
            w_wprintw("\bNew Game");
            break;
        case '2':
            w_wprintw("\bLoad Game");
            break;
        case '3':
            w_wprintw("\bReplay");
            break;
        case '4':
            exit(0);
            break;
        default:
            input(window, 1);
            break;
    }
    wrefresh(window);
}
void menu(WINDOW* window, struct GAME_T* GAME) {
    int i = input(window, 0);
    // if (in == -1) return;
    // menu(window, GAME);
}

void run(struct GAME_T* GAME) {
    printw("Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24");
    // paint_UI_2(GAME->ui_2.window, GAME);
    // paint_STATE(GAME->state.window, GAME);
    // paint_CONTROLS(GAME->controls.window, GAME);
    // paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    paint_HALL(GAME->hall_of_fame.window, GAME);
    paint_MENU(GAME->menu.window, GAME);

    refresh();

    menu(GAME->menu.window, GAME);
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
    for (int i = 1; i < POLE_COUNT + 1; i++) {
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
