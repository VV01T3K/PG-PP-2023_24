#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "define.h"
#include "headers.h"

void paint_STATE(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "State"););
    wrefresh(window);
}

void paint_HALL(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(1, 7, "Hall of Fame"););
    wrefresh(window);
}

void paint_CONTROLS(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    w_mvwprintw(getmaxy(window) - 1, 1, "Controls");
    w_mvwprintw(getmaxy(window) / 2, 4, "R(oll) | M(ove) | S(kip) | E(nd)");
    wrefresh(window);
}

void printDICE(WINDOW* win, int y, int x, struct GAME_T* GAME, int index) {
    if (GAME->dice[index] == 0) {
        mvwprintw(win, y, x, "%d-|#|", index + 1);
    } else if (GAME->dice[index] != -1) {
        mvwprintw(win, y, x, "%d-|%d|", index + 1, GAME->dice[index]);
    }
}

void paint_DICE(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    w_mvwprintw(getmaxy(window) - 1, 1, "Kostki");

    if (GAME->dice[1] == -1) {
        w_mvwprintw(2, 6, "Rzuć kostkami!");
        wrefresh(window);
        return;
    }

    printDICE(window, 2, 6, GAME, 0);
    printDICE(window, 2, 16, GAME, 1);
    printDICE(window, 3, 6, GAME, 2);
    printDICE(window, 3, 16, GAME, 3);

    if (GAME->dice[0] == GAME->dice[1]) {
        w_mvwprintw(4, 6, "Podwójny ruch!");
    }

    wrefresh(window);
}

void paint_MENU(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "Menu"););

    watrr(A_BOLD, w_mvwprintw(2, getmaxy(window) / 1.2, "Backgammon─1.0"););

    w_mvwprintw(5, 3, "( Enter x2 to confirm :v )");

    w_mvwprintw(8, 4, "1) New Game");
    w_mvwprintw(9, 4, "2) Load Game");
    w_mvwprintw(10, 4, "3) Replay");
    w_mvwprintw(11, 4, "4) Exit");

    wrefresh(window);
}

int menu(WINDOW* window, struct GAME_T* GAME) {
    char str[] = "Choose from list: ";
    w_mvwprintw(4, 3, str);
    char in[10];
    wgetnstr(window, in, 1);
    wmove(window, 4, 3 + strlen(str));
    int returner = atoi(in);
    switch (returner) {
        case 1:
            w_wprintw("New Game");
            break;
        case 2:
            w_wprintw("Load Game");
            break;
        case 3:
            w_wprintw("Replay");
            break;
        case 4:
            endwin();
            exit(0);
            break;
        default:
            w_wprintw("Wrong input!");
            return menu(window, GAME);
            break;
    }
    clearLine(4);
    wmove(window, 4, 3 + strlen(str));
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return menu(window, GAME);

    return returner;
}

void roll(struct GAME_T* GAME) {
    GAME->dice[0] = rand() % 6 + 1;
    GAME->dice[1] = rand() % 6 + 1;
    if (GAME->dice[0] == GAME->dice[1]) {
        GAME->dice[2] = GAME->dice[0];
        GAME->dice[3] = GAME->dice[0];
    } else {
        GAME->dice[2] = -1;
        GAME->dice[3] = -1;
    }
    paint_DICE(GAME->ui_2.window, GAME);
}

int decide(WINDOW* window, struct GAME_T* GAME, int gracz) {
    char in[10];
    wgetnstr(window, in, 1);
    clearLine(1);
    int returner = atoi(in);
    switch (returner) {
        case 1:
            roll(GAME);
            break;
        case 2:
            w_mvwprintw(1, 3, "Choose from list: Move");
            break;
        case 3:
            w_mvwprintw(1, 3, "Choose from list: Skip");
            break;
        case 4:
            w_mvwprintw(1, 3, "Choose from list: End");
            break;
        default:
            w_mvwprintw(1, 3, "Choose from list: Wrong input!");
            return decide(window, GAME, gracz);
            break;
    }
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return decide(window, GAME, gracz);
}

void turn(WINDOW* window, struct GAME_T* GAME, int gracz) {
    watrr(A_BOLD, atrrCLR(RED, mvwprintw(window, 1, 2, "You have to %s!",
                                         GAME->komunikat););
          atrrCLR(gracz == 1 ? MAGENTA : CYAN,
                  mvwprintw(window, 2, 2,
                            "Ruch: Gracza %s: ", gracz == 1 ? "A" : "B")););
    wrefresh(window);
    switch (decide(window, GAME, gracz)) {
        case 1:
            /* code */
            break;

        default:
            break;
    }
}

void gameplay(struct GAME_T* GAME) {
    while (1) {
        turn(GAME->controls.window, GAME, PLAYER_A);
        // check_win(GAME);
        // round(GAME->controls.window, GAME, PLAYER_B);
        // check_win(GAME);
    }
}

void run(struct GAME_T* GAME) {
    printw("Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24");
    paint_HALL(GAME->aside.window, GAME);
    paint_MENU(GAME->menu.window, GAME);

    refresh();

    switch (menu(GAME->menu.window, GAME)) {
        case 1:
            paint_STATE(GAME->aside.window, GAME);
            paint_DICE(GAME->ui_2.window, GAME);
            paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
            paint_CONTROLS(GAME->controls.window, GAME);
            gameplay(GAME);
            break;

        default:
            break;
    }
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

    GAME->dice[0] = -1;
    GAME->dice[1] = -1;

    strcpy(GAME->komunikat, "roll the dice");

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
