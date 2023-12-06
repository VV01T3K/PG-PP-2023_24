#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "headers.h"

void save_game(struct GAME_T* GAME, int gracz) {
    FILE* file = fopen("save.txt", "w");
    fprintf(file, "Zaczyna: %d | SEED: %d\n", gracz, GAME->rand_seed);
    fclose(file);
}
void save_turn(struct GAME_T* GAME, char* ruchy) {
    FILE* file = fopen("save.txt", "a");
    fprintf(file, "->%s\n", ruchy);
    fclose(file);
}
// ruch_add_num(struct GAME_T* GAME, int num) {
//     char num_str[MAX_SHORT_STR];
//     sprintf(num_str, " %d", num);
//     strcat(GAME->ruchy, num_str);
// }

void capture(struct GAME_T* GAME, int docelowe, int gracz);
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
        w_mvwprintw(4, 6, "Dublet!");
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
    clearLine(4);
    return result;
}

int roll(struct GAME_T* GAME) {
    GAME->dublet = FALSE;
    GAME->dice[0] = rand() % 6 + 1;
    GAME->dice[1] = rand() % 6 + 1;

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
    clearLine(getmaxy(window) / 2);
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
    watrr(A_BOLD, atrrCLR(kolor, mvwprintw(window, 1, 2, "%s %s",
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
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_dice(window, GAME, gracz);
    clearLine(2);
    return result;
}
void move_pionek(struct GAME_T* GAME, struct MOVE_T move, int gracz) {
    int pionek = move.pionek, kostka = move.kostka;
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
    char buffer[10];
    sprintf(buffer, " m %d %d", pionek, kostka);
    strcat(GAME->ruchy, buffer);
}

void capture(struct GAME_T* GAME, int docelowe, int gracz) {
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_A;
        BAR_PLAYER_B.liczba++;
    } else {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_B;
        BAR_PLAYER_A.liczba++;
    }
    GAME->plansza.pole[docelowe].liczba = 0;
}
int verify_move(struct GAME_T* GAME, struct MOVE_T move, int gracz) {
    int pionek = move.pionek, kostka = move.kostka;
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
        pause();
    }
    return 0;
}

int bar_empty(struct GAME_T* GAME, int gracz) {
    if (gracz == PLAYER_A) {
        if (BAR_PLAYER_A.liczba == 0) {
            return 1;
        }
    } else {
        if (BAR_PLAYER_B.liczba == 0) {
            return 1;
        }
    }
    return 0;
}

int asc_dice(const void* a, const void* b) { return (*(int*)a - *(int*)b); }
int desc_dice(const void* a, const void* b) { return (*(int*)b - *(int*)a); }

struct MOVE_T check_A_capture(struct GAME_T* GAME, int kolor, int kostki[4]) {
    struct MOVE_T move;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int j = 0; j < POLE_COUNT + 1; j++) {
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int docelowe = j + kostki[i];
                if (docelowe < 1 || docelowe > 24) continue;
                if (GAME->plansza.pole[docelowe].kolor != kolor &&
                    GAME->plansza.pole[docelowe].liczba == 1) {
                    move.kostka = i + 1;
                    move.pionek = j;
                    return move;
                }
            }
        }
    }
    move.kostka = -1;
    move.pionek = -1;
    return move;
}

struct MOVE_T check_B_capture(struct GAME_T* GAME, int kolor, int kostki[4]) {
    struct MOVE_T move;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int j = POLE_COUNT; j > 0; j--) {
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int docelowe = j - kostki[i];
                if (docelowe < 1 || docelowe > 24) continue;
                if (GAME->plansza.pole[docelowe].kolor != kolor &&
                    GAME->plansza.pole[docelowe].liczba == 1) {
                    move.kostka = i + 1;
                    move.pionek = j;
                    return move;
                }
            }
        }
    }
    move.kostka = -1;
    move.pionek = -1;
    return move;
}

int capture_possible(struct GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;

    int kostki[4] = {GAME->dice[0], GAME->dice[1], GAME->dice[2],
                     GAME->dice[3]};

    qsort(kostki, 4, sizeof(int), asc_dice);

    if (gracz == PLAYER_A) {
        if (check_A_capture(GAME, kolor, kostki).kostka != -1) {
            return 1;
        }
        return 0;

    } else {
        if (check_B_capture(GAME, kolor, kostki).kostka != -1) {
            return 1;
        }
        return 0;
    }
}

int enforce_move(struct MOVE_T forced, struct MOVE_T move, int bar_flag,
                 int capture_flag, int gracz, struct GAME_T* GAME) {
    if (capture_flag) {
        if (move.kostka != forced.kostka) {
            comms(GAME->controls.window, "wrong dice", RED, gracz);
            pause();
            return 1;
        }
        if (move.pionek != forced.pionek) {
            comms(GAME->controls.window, "wrong field", RED, gracz);
            pause();
            return 1;
        }
    }
    return 0;
}

int check_A_moves(struct GAME_T* GAME, int kolor, int kostki[4]) {
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int j = 0; j < POLE_COUNT + 1; j++) {
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int docelowe = j + kostki[i];
                if (docelowe < 1 || docelowe > 24) continue;
                if (GAME->plansza.pole[docelowe].kolor == kolor) {
                    return 1;
                } else if (GAME->plansza.pole[docelowe].liczba < 2) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int check_B_moves(struct GAME_T* GAME, int kolor, int kostki[4]) {
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int j = POLE_COUNT; j > 0; j--) {
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int docelowe = j - kostki[i];
                if (docelowe < 1 || docelowe > 24) continue;
                if (GAME->plansza.pole[docelowe].kolor == kolor) {
                    return 1;
                } else if (GAME->plansza.pole[docelowe].liczba < 2) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int move_possible(struct GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    int kostki[4] = {GAME->dice[0], GAME->dice[1], GAME->dice[2],
                     GAME->dice[3]};

    if (gracz == PLAYER_A)
        return check_A_moves(GAME, kolor, kostki);
    else
        return check_B_moves(GAME, kolor, kostki);
}

void move_action(WINDOW* window, struct GAME_T* GAME, int gracz) {
    char kostki[50];
    struct MOVE_T move, forced;
    int bar_flag = 0, capture_flag = 0;
    forced.kostka = -1;

    if (!move_possible(GAME, gracz)) {
        comms(window, "skip (no legal moves) [ANY]", RED, gracz);
        pause();
        GAME->pozostałe_ruchy = 0;
        return;
    }
    // bar
    if (!bar_empty(GAME, gracz)) {
        forced.pionek = gracz == PLAYER_A ? 0 : 25;
        move.pionek = forced.pionek;
        bar_flag = 1;
        comms(window, "move all pieces from bar! (ANY)", RED, gracz);
        pause();
    }
    // end bar

    // capture
    if (capture_possible(GAME, gracz)) {
        move = gracz == PLAYER_A
                   ? check_A_capture(GAME, CLR_PLAYER_A, GAME->dice)
                   : check_B_capture(GAME, CLR_PLAYER_B, GAME->dice);
        if (bar_flag && move.pionek == forced.pionek) {
            forced.kostka = move.kostka;
            capture_flag = 1;
        } else if (!bar_flag) {
            forced.kostka = move.kostka;
            forced.pionek = move.pionek;
            capture_flag = 1;
        }
        comms(window, "capture! [ANY]", RED, gracz);
        pause();
    }
    // end capture

    // manual
    move.kostka = -1;
    do {
        if (!bar_flag) {
            comms(window, "Wybierz pionek", GREEN, gracz);
            w_mvwprintw(3, 4, "Choose number from 1 to 24");
            clearLine(3);
            move.pionek = get_number(window, GAME, gracz);
        }

        comms(window, "Wybierz kostke", GREEN, gracz);
        w_mvwprintw(3, 4, "Choose number of dice from bottom right panel");
        clearLine(3);

        move.kostka = get_dice(window, GAME, gracz);

    } while (enforce_move(forced, move, bar_flag, capture_flag, gracz, GAME));

    // end manual

    // verify
    if (verify_move(GAME, move, gracz)) {
        pause();
        return move_action(window, GAME, gracz);
    }
    // end verify

    // move
    // start komunikat o ruchu
    if (move.pionek == 0 | move.pionek == 25) {
        sprintf(
            kostki, "Ruszyłeś pionka z bandy na pole %d",
            move.pionek + (gracz == PLAYER_A ? GAME->dice[move.kostka - 1]
                                             : -GAME->dice[move.kostka - 1]));
    } else {
        sprintf(
            kostki, "Ruszyłeś pionka z pola %d na pole %d", move.pionek,
            move.pionek + (gracz == PLAYER_A ? GAME->dice[move.kostka - 1]
                                             : -GAME->dice[move.kostka - 1]));
    }

    comms(window, kostki, GREEN, gracz);
    // end komunikat o ruchu

    move_pionek(GAME, move, gracz);
    pause();
}

void turn(WINDOW* window, struct GAME_T* GAME, int gracz) {
    comms(window, GAME->komunikat, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, "roll dice first", RED, gracz);
    }
    strcat(GAME->ruchy, " r");
    GAME->pozostałe_ruchy = roll(GAME);
    while (GAME->pozostałe_ruchy > 0) {
        comms(window, "You can move now", GREEN, gracz);
        while (decide_controls(window, GAME) != 'm') {
            comms(window, "move", RED, gracz);
        }
        move_action(window, GAME, gracz);
    }
    comms(window, "You can skip now", GREEN, gracz);
    while (decide_controls(window, GAME) != 's') {
        comms(window, "skip", RED, gracz);
    }
    strcat(GAME->ruchy, " s");
}
void gameplay(struct GAME_T* GAME, int gracz) {
    paint_DICE(GAME->ui_2.window, GAME);
    while (1) {
        if (gracz == PLAYER_A) {
            turn(GAME->controls.window, GAME, PLAYER_A);
            save_turn(GAME, GAME->ruchy);
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
        pause();
        return PLAYER_A;
    } else if (GAME->dice[0] < GAME->dice[1]) {
        comms(window, "Player B starts (Press Any key to continue)", GREEN,
              PLAYER_B);
        pause();
        return PLAYER_B;
    } else {
        comms(window, "Roll again", GREEN, PLAYER_A);
        pause();
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
            int gracz = PLAYER_A;
            initGame(GAME);
            save_game(GAME, gracz);
            gameplay(GAME, gracz);
            // gameplay(GAME, PLAYER_A);
            run(GAME);  // powrót do menu
            break;
        case 2:
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
    } pionki[] = {{1, 2, CLR_PLAYER_A},  {6, 5, CLR_PLAYER_B},
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
    for (int i = 1; i < POLE_COUNT + 1; i++) {
        GAME->plansza.pole[i].liczba = 0;
        GAME->plansza.pole[i].kolor = 0;
        GAME->plansza.pole[i].xyPos = 0;
        GAME->plansza.pole[i].number = i;
    }
    placePionki(GAME);
    BAR_PLAYER_A.liczba = 0;
    BAR_PLAYER_B.liczba = 0;

    BAR_PLAYER_A.kolor = CLR_PLAYER_A;
    BAR_PLAYER_B.kolor = CLR_PLAYER_B;

    GAME->plansza.dwor.gracz_A = 0;
    GAME->plansza.dwor.gracz_B = 0;

    GAME->dice[0] = -1;
    GAME->dice[1] = -1;
    GAME->dice[2] = -1;
    GAME->dice[3] = -1;

    strcpy(GAME->komunikat, "Good luck!");

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
    pause();
    endwin();
    return 0;
}
