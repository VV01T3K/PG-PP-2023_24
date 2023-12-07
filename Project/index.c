#include "Partials/headers.h"
void crud_move_pionek(struct GAME_T* GAME, int start, int cel, int gracz);
void multi_move(WINDOW* window, struct GAME_T* GAME, int gracz, int start);
void save_game(struct GAME_T* GAME, int gracz) {
    FILE* file = fopen(SAVE_PATH, "w");
    fprintf(file, "SEED: %d\n", GAME->rand_seed);
    fclose(file);
}
void save_turn(struct GAME_T* GAME, char* ruchy, char gracz) {
    FILE* file = fopen(SAVE_PATH, "a");
    fprintf(file, "->%c%s\n", gracz, ruchy);
    fclose(file);
}
void save_move(struct GAME_T* GAME, int start, int cel) {
    char buffer[20];
    sprintf(buffer, " m %d %d", start, cel);
    strcat(GAME->ruchy, buffer);
}

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

void paint_MENU(WINDOW* window, struct GAME_T* GAME) {
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

int decide_menu(WINDOW* window, struct GAME_T* GAME) {
    W_GETNSTR_IN(1, 4, MENU_PADD);
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
            w_wprintw(TXT_WRONG);
            return decide_menu(window, GAME);
            break;
    }
    clearLine(4);
    wmove(window, 4, MENU_PADD);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return decide_menu(window, GAME);
    clearLine(4);
    return result;
}

int roll(struct GAME_T* GAME) {
    GAME->dublet = FALSE;
    // GAME->dice[0] = rand() % 6 + 1;
    // GAME->dice[1] = rand() % 6 + 1;
    GAME->dice[0] = 3;
    GAME->dice[1] = 3;

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
    w_mvwprintw(getmaxy(window) / 2, 4, TXT_CONTROLS);
    clearLine(getmaxy(window) / 2);
    W_GETNSTR_IN(1, 2, CONTROLS_PADD);
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
            w_wprintw(TXT_WRONG);
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
    W_GETNSTR_IN(2, 2, CONTROLS_PADD)
    int result = atoi(in);
    if (result < 1 || result > 24) {
        w_wprintw(TXT_WRONG);
        return get_number(window, GAME, gracz);
    }
    wprintw(window, "Pole Nr %d", result);
    clearLine(2);
    wmove(window, 2, CONTROLS_PADD);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_number(window, GAME, gracz);
    clearLine(2);
    return result;
}

int get_dice(WINDOW* window, struct GAME_T* GAME, int gracz, int multi,
             int start) {
    W_GETNSTR_IN(1, 2, CONTROLS_PADD)
    if (in[0] == 'm' && multi == MULTI_ON) {
        multi_move(window, GAME, gracz, start);
        return -10;
    }
    int result = atoi(in);
    if (result < 1 || result > (GAME->dice[3] != -1 ? 4 : 2)) {
        w_wprintw(TXT_WRONG);
        clearLine(2);
        return get_dice(window, GAME, gracz, multi, start);
    }
    if (GAME->dice[result - 1] == 0) {
        w_wprintw(TXT_DICE_USED);
        return get_dice(window, GAME, gracz, multi, start);
    }

    wprintw(window, TXT_DICE_NR, result);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_dice(window, GAME, gracz, multi, start);
    clearLine(2);
    return result;
}
void move_pionek(struct GAME_T* GAME, struct MOVE_T move, int gracz) {
    int pionek = move.pionek, kostka = move.kostka;
    int cel = pionek + gracz_step(kostka - 1);
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[pionek].liczba--;
        GAME->plansza.pole[cel].liczba++;
        GAME->plansza.pole[cel].kolor = CLR_PLAYER_A;

    } else {
        GAME->plansza.pole[pionek].liczba--;
        GAME->plansza.pole[cel].liczba++;
        GAME->plansza.pole[cel].kolor = CLR_PLAYER_B;
    }
    GAME->dice[kostka - 1] = 0;
    paint_DICE(GAME->ui_2.window, GAME);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    GAME->pozostałe_ruchy--;
    save_move(GAME, pionek, cel);
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
int verify_move(struct GAME_T* GAME, int start, int cel, int gracz, int multi) {
    WINDOW* win = GAME->controls.window;
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    if (GAME->plansza.pole[start].kolor != kolor) {
        comms(win, TXT_VERIFY_1, RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[start].liczba == 0) {
        comms(win, TXT_VERIFY_2, RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[cel].kolor != kolor &&
        GAME->plansza.pole[cel].liczba > 1) {
        comms(win, TXT_VERIFY_3, RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[cel].kolor != kolor &&
        GAME->plansza.pole[cel].liczba == 1) {
        if (multi == MULTI_OFF) {
            capture(GAME, cel, gracz);
            comms(win, TXT_CAPTURE, GREEN, gracz);
            pause();
        }
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

int enforce_move(struct MOVE_T forced, struct MOVE_T move, int capture_flag,
                 int gracz, struct GAME_T* GAME) {
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

void multi_move(WINDOW* window, struct GAME_T* GAME, int gracz, int start) {
    comms(window, "CHOOSE DICES [np.: 2+1+3]", GREEN, gracz);
    W_GETNSTR_IN(7, 2, CONTROLS_PADD);
    int kostki[4] = {0};
    char* split = strtok(in, "+");
    int j, i = 0, return_flag = 0;

    while (split != NULL) {
        kostki[i++] = atoi(split);
        split = strtok(NULL, "+");
    }

    for (i = 0; i < 4; i++) {
        if (!kostki[i]) continue;

        if (kostki[i] < 1 || kostki[i] > (GAME->dublet ? 4 : 2)) {
            w_wprintw("WRONG DICE NUMBERS");
            return_flag = 1;
        }
        if (GAME->dice[kostki[i] - 1] == 0) {
            w_wprintw("DICE ALREADY USED");
            return_flag = 1;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) continue;
            if (kostki[i] == kostki[j]) {
                w_wprintw("DICE USED TWICE");
                return_flag = 1;
                break;
            }
        }
        if (return_flag) return multi_move(window, GAME, gracz, start);
    }
    int cel;
    for (i = 0, cel = start; i < 4; i++) {
        if (!kostki[i]) continue;
        cel += gracz_step(kostki[i] - 1);
        if (verify_move(GAME, start, cel, gracz, MULTI_OFF)) {
            pause();
            return multi_move(window, GAME, gracz, start);
        }
    }
    for (i = 0, cel = start; i < 4; i++) {
        if (!kostki[i]) continue;
        int prev = cel;
        cel += gracz_step(kostki[i] - 1);
        verify_move(GAME, start, cel, gracz, MULTI_ON);
        GAME->dice[kostki[i] - 1] = 0;
        GAME->pozostałe_ruchy--;
        save_move(GAME, prev, cel);
    }
    paint_DICE(GAME->ui_2.window, GAME);
    crud_move_pionek(GAME, start, cel, gracz);
}

void move_action(WINDOW* window, struct GAME_T* GAME, int gracz) {
    char kostki[50];
    struct MOVE_T move, forced;
    int bar_flag = 0, capture_flag = 0;
    forced.kostka = -1;

    if (!move_possible(GAME, gracz)) {
        comms(window, TXT_MOVE_IMP, RED, gracz);
        pause();
        GAME->pozostałe_ruchy = 0;
        return;
    }
    // bar
    if (!bar_empty(GAME, gracz)) {
        forced.pionek = gracz == PLAYER_A ? 0 : 25;
        move.pionek = forced.pionek;
        bar_flag = 1;
        comms(window, TXT_BAR_FULL, RED, gracz);
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
        comms(window, TXT_CAP_POS, RED, gracz);
        pause();
    }
    // end capture

    // manual
    move.kostka = -1;
    do {
        if (!bar_flag) {
            comms(window, TXT_M_PION, GREEN, gracz);
            w_mvwprintw(3, 4, TXT_M_FIELD);
            clearLine(3);
            move.pionek = get_number(window, GAME, gracz);
        }

        comms(window, TXT_M_DICE, GREEN, gracz);
        w_mvwprintw(3, 4, TXT_M_DICE_INFO);
        clearLine(3);
        move.kostka =
            get_dice(window, GAME, gracz, (capture_flag ? MULTI_OFF : MULTI_ON),
                     move.pionek);
        if (move.kostka == -10) return;

    } while (enforce_move(forced, move, capture_flag, gracz, GAME));

    // end manual

    // verify
    int cel = move.pionek + gracz_step(move.kostka - 1);
    if (verify_move(GAME, move.pionek, cel, gracz, MULTI_OFF)) {
        pause();
        return move_action(window, GAME, gracz);
    }

    // end verify

    // move
    // start komunikat o ruchu
    if (move.pionek == 0 | move.pionek == 25) {
        sprintf(kostki, TXT_POST_MOVE_BAR,
                move.pionek + gracz_step(move.kostka - 1));
    } else {
        sprintf(kostki, TXT_POST_MOVE, move.pionek,
                move.pionek + gracz_step(move.kostka - 1));
    }

    comms(window, kostki, GREEN, gracz);
    // end komunikat o ruchu
    move_pionek(GAME, move, gracz);
}

void turn(WINDOW* window, struct GAME_T* GAME, int gracz) {
    comms(window, GAME->komunikat, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, TXT_DECIDE_R, RED, gracz);
    }
    GAME->pozostałe_ruchy = roll(GAME);
    while (GAME->pozostałe_ruchy > 0) {
        comms(window, TXT_DECIDE_M, GREEN, gracz);
        while (decide_controls(window, GAME) != 'm') {
            comms(window, "move", RED, gracz);
        }
        move_action(window, GAME, gracz);
    }
    comms(window, TXT_DECIDE_S, GREEN, gracz);
    while (decide_controls(window, GAME) != 's') {
        comms(window, "skip", RED, gracz);
    }
}
void gameplay(struct GAME_T* GAME, int gracz) {
    paint_DICE(GAME->ui_2.window, GAME);
    while (1) {
        if (gracz == PLAYER_A) {
            turn(GAME->controls.window, GAME, PLAYER_A);
            save_turn(GAME, GAME->ruchy, 'A');
            // check_win(GAME);
            turn(GAME->controls.window, GAME, PLAYER_B);
            save_turn(GAME, GAME->ruchy, 'B');
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
    comms(window, TXT_START_ROLL, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, TXT_START_ROLL, RED, gracz);
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
        comms(window, TXT_START_PLAYER_A, GREEN, PLAYER_A);
        pause();
        return PLAYER_A;
    } else if (GAME->dice[0] < GAME->dice[1]) {
        comms(window, TXT_START_PLAYER_B, GREEN, PLAYER_B);
        pause();
        return PLAYER_B;
    } else {
        comms(window, "Roll again", GREEN, PLAYER_A);
        pause();
        return who_starts(GAME);
    }
}

void crud_move_pionek(struct GAME_T* GAME, int start, int cel, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    if (GAME->plansza.pole[cel].kolor != 0 &&
        GAME->plansza.pole[cel].kolor != kolor) {
        capture(GAME, cel, gracz);
    }
    GAME->plansza.pole[cel].kolor = kolor;

    GAME->plansza.pole[cel].liczba++;
    GAME->plansza.pole[start].liczba--;
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
}

int load_save(struct GAME_T* GAME) {
    FILE* file = fopen(SAVE_PATH, "r+");
    char buffer[2][3], c;
    fscanf(file, "SEED: %d", &GAME->rand_seed);
    int gracz = 0;
    while (fscanf(file, "%c", &c) != EOF) {
        // nadpisanie tekstu
        // if (c == 't') {
        //     fseek(file, -1, SEEK_CUR);
        //     fprintf(file, "Your text here1\n");
        //     fprintf(file, "Your text here2\n");
        // }
        if (c == '\n') {
            fscanf(file, "->%c", &c);
            if (c == 'A') {
                gracz = PLAYER_A;
            } else if (c == 'B') {
                gracz = PLAYER_B;
            }
        }
        if (c == 'm') {
            int pionek, cel;
            fscanf(file, "%s %s", buffer[0], buffer[1]);
            pionek = atoi(buffer[0]);
            cel = atoi(buffer[1]);
            crud_move_pionek(GAME, pionek, cel, gracz);
        }
    }
    fclose(file);
    return gracz == PLAYER_A ? PLAYER_B : PLAYER_A;
}
void run(struct GAME_T* GAME) {
    int gracz;
    paint_HALL(GAME->aside.window, GAME);
    paint_MENU(GAME->menu.window, GAME);

    refresh();

    switch (decide_menu(GAME->menu.window, GAME)) {
        case 1:
            paint_GAMEVIEW(GAME);
            // int gracz = who_starts(GAME);
            gracz = PLAYER_A;
            initGame(GAME);
            save_game(GAME, gracz);
            gameplay(GAME, gracz);
            // gameplay(GAME, PLAYER_A);
            run(GAME);  // powrót do menu
            break;
        case 2:
            paint_GAMEVIEW(GAME);
            initGame(GAME);
            gracz = load_save(GAME);
            gameplay(GAME, gracz);
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
    } pionki[] = {{1, 2, CLR_PLAYER_A},  {6, 1, CLR_PLAYER_B},
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

    strcpy(GAME->komunikat, TXT_TURN);

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
        printw(TXT_AUTHOR);
        initWindows(GAME);
        initGame(GAME);
        run(GAME);
    }
    free(GAME);
    pause();
    endwin();
    return 0;
}
