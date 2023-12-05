#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>

#include "define.h"
#include "headers.h"
void initGame(struct GAME_T* GAME);
void run(struct GAME_T* GAME);
void paint_GAMEVIEW(struct GAME_T* GAME);

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

void paint_CONTROLS(WINDOW* window, struct GAME_T* GAME, int gracz) {
    wclear(window);
    box(window, 0, 0);
    w_mvwprintw(getmaxy(window) - 1, 1, "Controls");

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

    if (GAME->dice[0] == -1) {
        w_mvwprintw(2, 6, "Rzuć kostkami!");
        wrefresh(window);
        return;
    }

    printDICE(window, 2, 6, GAME, 0);
    printDICE(window, 2, 16, GAME, 1);
    printDICE(window, 3, 6, GAME, 2);
    printDICE(window, 3, 16, GAME, 3);

    if (GAME->dice[0] == GAME->dice[1] && GAME->status == PLAYING) {
        w_mvwprintw(4, 6, "Podwójny ruch!");
    }

    wrefresh(window);
}

void paint_MENU(WINDOW* window, struct GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(0, 1, "Menu"););

    watrr(A_BOLD, w_mvwprintw(2, getmaxy(window) / 1.2, "Backgammon─1.0"););

    w_mvwprintw(4, 3, "Choose from list: ");
    w_mvwprintw(5, 3, "( Enter x2 to confirm :v )");

    w_mvwprintw(8, 4, "1) New Game");
    w_mvwprintw(9, 4, "2) Load Game");
    w_mvwprintw(10, 4, "3) Replay");
    w_mvwprintw(11, 4, "4) Exit");

    wrefresh(window);
}

int decide_menu(WINDOW* window, struct GAME_T* GAME) {
    W_GETNSTR_IN(1, 4, menu_padd);
    int result = atoi(in);
    switch (result) {
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
            return decide_menu(window, GAME);
            break;
    }
    clearLine(4);
    wmove(window, 4, menu_padd);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return decide_menu(window, GAME);

    return result;
}

int roll(struct GAME_T* GAME) {
    GAME->dice[0] = rand() % 6 + 1;
    GAME->dice[1] = rand() % 6 + 1;
    if (GAME->dice[0] == GAME->dice[1]) {
        GAME->dice[2] = GAME->dice[0];
        GAME->dice[3] = GAME->dice[0];
        paint_DICE(GAME->ui_2.window, GAME);
        return 4;
    } else {
        GAME->dice[2] = -1;
        GAME->dice[3] = -1;
        paint_DICE(GAME->ui_2.window, GAME);
        return 2;
    }
}

int decide_controls(WINDOW* window, struct GAME_T* GAME) {
    w_mvwprintw(getmaxy(window) / 2, 4, controls_promt);
    W_GETNSTR_IN(1, 2, controls_padd);
    switch (tolower(in[0])) {
        case 'r':
            w_wprintw("Roll");
            break;
        case 'm':
            w_wprintw("Move");
            break;
        case 's':
            w_wprintw("Skip");
            break;
        case 'e':
            w_wprintw("Exit");
            run(GAME);
            break;
        default:
            w_wprintw("Wrong input!");
            return decide_controls(window, GAME);
            break;
    }
    char ch = wgetch(window);
    if (ch != '\n') return decide_controls(window, GAME);
    clearLine(2);
    w_mvwprintw(2, 0, "│");
    return tolower(in[0]);
}

void comms(WINDOW* window, char* str, int kolor, int gracz) {
    wmove(window, 1, 2);
    clearLine(1);
    char prefix[] = "You have to";
    watrr(A_BOLD, atrrCLR(kolor, mvwprintw(window, 1, 2, "%s %s!",
                                           kolor == RED ? prefix : "\b", str););
          atrrCLR(gracz == 1 ? MAGENTA : CYAN,
                  mvwprintw(window, 2, 2,
                            "Ruch-Gracza %s: ", gracz == 1 ? "A" : "B")););
    wrefresh(window);
}

int get_number(WINDOW* window, struct GAME_T* GAME, int gracz) {
    W_GETNSTR_IN(2, 2, controls_padd)
    int result = atoi(in);
    if (result < 1 || result > 24) {
        w_wprintw("Wrong input!");
        return get_number(window, GAME, gracz);
    }
    clearLine(2);
    wmove(window, 2, controls_padd);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_number(window, GAME, gracz);
    clearLine(2);
    w_mvwprintw(2, 0, "│");
    return result;
}

int move_action(WINDOW* window, struct GAME_T* GAME, int gracz) {
    int wykonane_ruchy = 1;
    comms(window, "Wykonano ruch", GREEN, gracz);

    // check_bar(GAME, gracz);
    // check_zbicie(GAME, gracz);
    // check_dwor(GAME, gracz);

    comms(window, "Wybierz pionek", GREEN, gracz);
    w_mvwprintw(getmaxy(window) / 2, 4, "Choose number from 1 to 24         ");
    int pionek = get_number(window, GAME, gracz);

    // decide_move(window, GAME, gracz);

    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    return wykonane_ruchy;
}

void turn(WINDOW* window, struct GAME_T* GAME, int gracz) {
    comms(window, GAME->komunikat, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, "roll dice first", RED, gracz);
    }
    int kostki = roll(GAME);
    comms(window, "You can move now", GREEN, gracz);
    while (kostki > 0) {
        while (decide_controls(window, GAME) != 'm') {
            comms(window, "move", RED, gracz);
        }
        kostki -= move_action(window, GAME, gracz);
    }
    comms(window, "You can skip now", GREEN, gracz);
    while (decide_controls(window, GAME) != 's') {
        comms(window, "skip", RED, gracz);
    }
}
void gameplay(struct GAME_T* GAME, int gracz) {
    GAME->status = PLAYING;
    paint_DICE(GAME->ui_2.window, GAME);
    while (1) {
        if (gracz == PLAYER_A) {
            turn(GAME->controls.window, GAME, PLAYER_A);
            // check_win(GAME);
            turn(GAME->controls.window, GAME, PLAYER_B);
            // check_win(GAME);
        } else {
            turn(GAME->controls.window, GAME, PLAYER_B);
            // check_win(GAME);
            turn(GAME->controls.window, GAME, PLAYER_A);
            // check_win(GAME);
        }
    }
}

void paint_GAMEVIEW(struct GAME_T* GAME) {
    paint_STATE(GAME->aside.window, GAME);
    paint_DICE(GAME->ui_2.window, GAME);
    paint_CONTROLS(GAME->controls.window, GAME, PLAYER_A);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
}
void starting_roll(struct GAME_T* GAME, WINDOW* window, int gracz) {
    comms(window, "Roll to decide who starts", GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, "roll to decide who starts", RED, gracz);
    }
    GAME->dice[gracz - 1] = rand() % 6 + 1;
    paint_DICE(GAME->ui_2.window, GAME);
    wrefresh(window);
}
int who_starts(struct GAME_T* GAME) {
    WINDOW* window = GAME->controls.window;
    starting_roll(GAME, window, PLAYER_A);
    starting_roll(GAME, window, PLAYER_B);
    if (GAME->dice[0] > GAME->dice[1]) {
        comms(window, "Player A starts (Press Any key to continue)", GREEN,
              PLAYER_A);
        getch();
        return PLAYER_A;
    } else if (GAME->dice[0] < GAME->dice[1]) {
        comms(window, "Player B starts (Press Any key to continue)", GREEN,
              PLAYER_B);
        getch();
        return PLAYER_B;
    } else {
        comms(window, "Roll again", GREEN, PLAYER_A);
        getch();
        return who_starts(GAME);
    }
}

void run(struct GAME_T* GAME) {
    paint_HALL(GAME->aside.window, GAME);
    paint_MENU(GAME->menu.window, GAME);

    refresh();

    switch (decide_menu(GAME->menu.window, GAME)) {
        case 1:
            paint_GAMEVIEW(GAME);
            int gracz = who_starts(GAME);
            initGame(GAME);
            gameplay(GAME, gracz);
            run(GAME);
            break;
        case 2:
            // load_save(GAME);
            paint_GAMEVIEW(GAME);
            gameplay(GAME, PLAYER_B);
            break;
        case 3:
            paint_STATE(GAME->aside.window, GAME);
            paint_DICE(GAME->ui_2.window, GAME);
            paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
            // paint_REPLAY(GAME->controls.window, GAME, PLAYER_A);
            // replay(GAME);
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
    GAME->status = STARTED;

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
    GAME->dice[2] = -1;
    GAME->dice[3] = -1;

    strcpy(GAME->komunikat, "Good luck and have fun!");

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
        printw("Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24");
        initWindows(GAME);
        initGame(GAME);
        run(GAME);
    }
    getch();
    endwin();
    return 0;
}
