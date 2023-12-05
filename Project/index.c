#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "headers.h"
void capture(struct GAME_T* GAME, int docelowe, int gracz);
void save_game(struct GAME_T* GAME, int gracz) {
    fprintf(GAME->save, "Zaczyna: %d | SEED: %d\n", gracz, GAME->rand_seed);
}
void save_turn(struct GAME_T* GAME, char* ruchy) {
    fprintf(GAME->save, "->%s\n", ruchy);
}
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

    if (GAME->dublet) {
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
    GAME->dublet = FALSE;
    // GAME->dice[0] = rand() % 6 + 1;
    // GAME->dice[1] = rand() % 6 + 1;
    GAME->dice[0] = 5;
    GAME->dice[1] = 5;

    if (GAME->dice[0] == GAME->dice[1]) {
        GAME->dice[2] = GAME->dice[0];
        GAME->dice[3] = GAME->dice[0];
        GAME->dublet = TRUE;
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
    return tolower(in[0]);
}

void comms(WINDOW* window, char* str, int kolor, int gracz) {
    wmove(window, 1, 2);
    clearLine(1);
    char prefix[] = "You have to";
    watrr(A_BOLD, atrrCLR(kolor, mvwprintw(window, 1, 2, "%s %s!",
                                           kolor == RED ? prefix : "\b", str););
          atrrCLR(gracz == 1 ? CLR_PLAYER_A : CLR_PLAYER_B,
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
    wprintw(window, "Pole Nr %d", result);
    clearLine(2);
    wmove(window, 2, controls_padd);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_number(window, GAME, gracz);
    clearLine(2);
    return result;
}

int get_dice(WINDOW* window, struct GAME_T* GAME, int gracz) {
    W_GETNSTR_IN(1, 2, controls_padd)
    int result = atoi(in);
    if (result < 1 || result > (GAME->dice[3] != -1 ? 4 : 2)) {
        w_wprintw("Wrong input!");
        clearLine(2);
        return get_dice(window, GAME, gracz);
    }
    if (GAME->dice[result - 1] == 0) {
        w_wprintw("Dice already used!");
        return get_dice(window, GAME, gracz);
    }

    wprintw(window, "Kostka Nr %d", result);
    clearLine(2);
    wmove(window, 2, controls_padd);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_dice(window, GAME, gracz);
    clearLine(2);
    return result;
}
void move_pionek(struct GAME_T* GAME, int pionek, int kostka, int gracz) {
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[pionek].liczba--;
        GAME->plansza.pole[pionek + GAME->dice[kostka - 1]].liczba++;
        GAME->plansza.pole[pionek + GAME->dice[kostka - 1]].kolor =
            CLR_PLAYER_A;

    } else {
        GAME->plansza.pole[pionek].liczba--;
        GAME->plansza.pole[pionek - GAME->dice[kostka - 1]].liczba++;
        GAME->plansza.pole[pionek - GAME->dice[kostka - 1]].kolor =
            CLR_PLAYER_B;
    }
    GAME->dice[kostka - 1] = 0;
    paint_DICE(GAME->ui_2.window, GAME);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    GAME->pozostałe_ruchy--;
}

int verify_bar_move(struct GAME_T* GAME, int kostka, int gracz) {
    int docelowe = gracz == PLAYER_A ? GAME->dice[kostka - 1]
                                     : POLE_COUNT - GAME->dice[kostka - 1];
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;

    if (GAME->plansza.pole[docelowe].kolor != kolor &&
        GAME->plansza.pole[docelowe].liczba > 1) {
        comms(GAME->controls.window, "choose conquerable field", RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[docelowe].kolor != kolor &&
        GAME->plansza.pole[docelowe].liczba == 1) {
        capture(GAME, docelowe, gracz);
        comms(GAME->controls.window, "Ładne bicie", GREEN, gracz);
        getch();
    }
    return 0;
}

void bar_move(struct GAME_T* GAME, int kostka, int gracz) {
    if (gracz == PLAYER_A) {
        GAME->plansza.bar.gracz_A--;
        GAME->plansza.pole[GAME->dice[kostka - 1]].liczba++;
        GAME->plansza.pole[GAME->dice[kostka - 1]].kolor = CLR_PLAYER_A;
    } else {
        GAME->plansza.bar.gracz_B--;
        GAME->plansza.pole[POLE_COUNT - GAME->dice[kostka - 1]].liczba++;
        GAME->plansza.pole[POLE_COUNT - GAME->dice[kostka - 1]].kolor =
            CLR_PLAYER_B;
    }
    GAME->dice[kostka - 1] = 0;
    paint_DICE(GAME->ui_2.window, GAME);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    GAME->pozostałe_ruchy--;
}

int forced_bar(WINDOW* window, struct GAME_T* GAME, int gracz) {
    if (gracz == PLAYER_A) {
        if (!GAME->plansza.bar.gracz_A > 0) {
            return 0;
        }
    } else if (gracz == PLAYER_B) {
        if (!GAME->plansza.bar.gracz_B > 0) {
            return 0;
        }
    }
    comms(window, "move from bar [ENTER]", RED, gracz);
    getch();
    comms(window, "Wybierz kostke", GREEN, gracz);
    w_mvwprintw(3, 4, "Choose number of dice from bottom right panel");
    clearLine(3);
    int kostka = get_dice(window, GAME, gracz);
    if (verify_bar_move(GAME, kostka, gracz)) {
        getch();
        return forced_bar(window, GAME, gracz);
    }
    bar_move(GAME, kostka, gracz);
    return 1;
}

int forced_move(struct GAME_T* GAME, int gracz) {
    WINDOW* window = GAME->controls.window;
    while (forced_bar(window, GAME, gracz)) {
    };
    // if (forced_dwor(GAME, gracz)) return 1;
    return 0;
}
void capture(struct GAME_T* GAME, int docelowe, int gracz) {
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_A;
        GAME->plansza.bar.gracz_B++;
    } else {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_B;
        GAME->plansza.bar.gracz_A++;
    }
    GAME->plansza.pole[docelowe].liczba = 0;
}
int verify_move(struct GAME_T* GAME, int pionek, int kostka, int gracz) {
    int docelowe = pionek + (gracz == PLAYER_A ? GAME->dice[kostka - 1]
                                               : -GAME->dice[kostka - 1]);
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    if (GAME->plansza.pole[pionek].kolor != kolor) {
        comms(GAME->controls.window, "choose your field", RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[pionek].liczba == 0) {
        comms(GAME->controls.window, "choose field with pieces", RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[docelowe].kolor != kolor &&
        GAME->plansza.pole[docelowe].liczba > 1) {
        comms(GAME->controls.window, "choose conquerable field", RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[docelowe].kolor != kolor &&
        GAME->plansza.pole[docelowe].liczba == 1) {
        capture(GAME, docelowe, gracz);
        comms(GAME->controls.window, "Ładne bicie", GREEN, gracz);
        getch();
    }
    return 0;
}
void move_action(WINDOW* window, struct GAME_T* GAME, int gracz) {
    comms(window, "Wykonano ruch", GREEN, gracz);

    if (forced_move(GAME, gracz)) return;

    comms(window, "Wybierz pionek", GREEN, gracz);
    w_mvwprintw(3, 4, "Choose number from 1 to 24");
    clearLine(3);
    int pionek = get_number(window, GAME, gracz);
    comms(window, "Wybierz kostke", GREEN, gracz);
    w_mvwprintw(3, 4, "Choose number of dice from bottom right panel");
    clearLine(3);
    int kostka = get_dice(window, GAME, gracz);

    if (verify_move(GAME, pionek, kostka, gracz)) {
        getch();
        return move_action(window, GAME, gracz);
    }

    char kostki[50];
    sprintf(kostki, "Ruszyłeś pionka z pola %d na pole %d", pionek,
            pionek + (gracz == PLAYER_A ? GAME->dice[kostka - 1]
                                        : -GAME->dice[kostka - 1]));
    comms(window, kostki, GREEN, gracz);

    move_pionek(GAME, pionek, kostka, gracz);
}

void turn(WINDOW* window, struct GAME_T* GAME, int gracz) {
    char ruchy[100];
    comms(window, GAME->komunikat, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, "roll dice first", RED, gracz);
    }
    strcat(ruchy, " r");
    GAME->pozostałe_ruchy = roll(GAME);
    comms(window, "You can move now", GREEN, gracz);
    while (GAME->pozostałe_ruchy > 0) {
        while (decide_controls(window, GAME) != 'm') {
            comms(window, "move", RED, gracz);
        }
        strcat(ruchy, " m");
        move_action(window, GAME, gracz);
    }
    comms(window, "You can skip now", GREEN, gracz);
    while (decide_controls(window, GAME) != 's') {
        comms(window, "skip", RED, gracz);
    }
    strcat(ruchy, " s");
    save_turn(GAME, ruchy);
}
void gameplay(struct GAME_T* GAME, int gracz) {
    paint_DICE(GAME->ui_2.window, GAME);
    // while (1) {
    //     if (gracz == PLAYER_A) {
    turn(GAME->controls.window, GAME, PLAYER_A);
    //         check_win(GAME);
    turn(GAME->controls.window, GAME, PLAYER_B);
    //         check_win(GAME);
    //     } else {
    //         turn(GAME->controls.window, GAME, PLAYER_B);
    //         check_win(GAME);
    //         turn(GAME->controls.window, GAME, PLAYER_A);
    //         check_win(GAME);
    //     }
    // }
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
            // int gracz = who_starts(GAME);
            // save_game(GAME, gracz);
            initGame(GAME);
            // gameplay(GAME, gracz);
            gameplay(GAME, PLAYER_A);
            run(GAME);
            break;
        case 2:
            // char ruchy[100];
            // save_game(GAME, PLAYER_B);
            // strcpy(ruchy, "r m 10 2+1");
            // save_turn(GAME, ruchy);
            // strcpy(ruchy, "r m 15 2+1");
            // save_turn(GAME, ruchy);
            // strcpy(ruchy, "r m 20 3");
            // save_turn(GAME, ruchy);
            // load_save(GAME);
            // paint_GAMEVIEW(GAME);
            // gameplay(GAME, PLAYER_B);
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
    } pionki[] = {{1, 2, CLR_PLAYER_A},  {6, 1, CLR_PLAYER_B},  // 5 -> 1
                  {8, 3, CLR_PLAYER_B},  {12, 5, CLR_PLAYER_A},
                  {13, 5, CLR_PLAYER_B}, {17, 3, CLR_PLAYER_A},
                  {19, 5, CLR_PLAYER_A}, {24, 2, CLR_PLAYER_B}};

    for (int i = 0; i < sizeof(pionki) / sizeof(pionki[0]); i++) {
        GAME->plansza.pole[pionki[i].index].liczba = pionki[i].liczba;
        GAME->plansza.pole[pionki[i].index].kolor = pionki[i].kolor;
    }
}

void initGame(struct GAME_T* GAME) {
    GAME->rand_seed = time(NULL);
    srand(GAME->rand_seed);
    GAME->save = fopen("savedGame.txt", "w");

    for (int i = 1; i < POLE_COUNT + 1; i++) {
        GAME->plansza.pole[i].liczba = 0;
        GAME->plansza.pole[i].kolor = 0;
        GAME->plansza.pole[i].xyPos = 0;
        GAME->plansza.pole[i].number = i;
    }
    placePionki(GAME);
    GAME->plansza.bar.gracz_A = 2;
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
    struct GAME_T* GAME = (struct GAME_T*)malloc(sizeof(struct GAME_T));
    if (GAME == NULL) return 1;
    if (initialInit(GAME)) {
        printw("Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24");
        initWindows(GAME);
        initGame(GAME);
        run(GAME);
    }
    getch();
    endwin();
    return 0;
}
