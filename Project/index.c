#include "Partials/headers.h"
void crud_move_pionek(GAME_T* GAME, int start, int cel, int gracz);
void multi_move(WINDOW* window, GAME_T* GAME, int gracz, int start);
void clear_save() {
    FILE* file = fopen(SAVE_PATH, "w");
    fclose(file);
}
void save_game(GAME_T* GAME) {
    FILE* file = fopen(SAVE_PATH, "a");
    fprintf(file, "SEED: %d | SCORE: A%d B%d\n", GAME->rand_seed,
            GAME->gracz_A.wynik, GAME->gracz_B.wynik);
    fclose(file);
}
void paint_DICE(WINDOW* window, GAME_T* GAME);
void save_turn(GAME_T* GAME, char* ruchy, char gracz) {
    FILE* file = fopen(SAVE_PATH, "a");
    fprintf(file, "->%d%c %d%s\n", GAME->turn, gracz, GAME->home_news, ruchy);
    ruchy[0] = '\0';
    fclose(file);
}
void save_move(GAME_T* GAME, int start, int cel) {
    char buffer[20];
    sprintf(buffer, " m %d %d", start, cel);
    strcat(GAME->ruchy, buffer);
}
void exec_win(GAME_T* GAME, int points, int gracz);
void capture(GAME_T* GAME, int docelowe, int gracz);
void initGame(GAME_T* GAME);
void run(GAME_T* GAME);
void paint_GAMEVIEW(GAME_T* GAME);

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
    mvwprintw(window, 6, 18, "%d", GAME->pozostałe_ruchy);

    wmove(window, 7, 4);
    for (int i = 0; i < 18; i++) w_wprintw(HR);
    wrefresh(window);
    paint_DICE(GAME->ui_2.window, GAME);
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

int decide_menu(WINDOW* window, GAME_T* GAME) {
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
            free(GAME);
            endwin();
            exit(0);
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

int roll(GAME_T* GAME) {
    GAME->dublet = FALSE;
    GAME->dice[0] = 1;
    GAME->dice[1] = 2;
    // GAME->dice[0] = rand() % 6 + 1;
    // GAME->dice[1] = rand() % 6 + 1;

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

void paint_NAME(WINDOW* window, GAME_T* GAME) {
    wclear(window);
    box(window, 0, 0);
    watrr(A_BOLD, w_mvwprintw(1, 1, TXT_NAME););
    wrefresh(window);
}

void get_name(char* name, WINDOW* window, GAME_T* GAME) {
    W_GETNSTR_IN(MAX_NAME, 1, strlen(TXT_NAME) + 2);
    strcpy(name, in);
}

int asc_fame(const void* a, const void* b) {
    return ((GRACZ_T*)a)->wynik - ((GRACZ_T*)b)->wynik;
}
int desc_fame(const void* a, const void* b) {
    return ((GRACZ_T*)b)->wynik - ((GRACZ_T*)a)->wynik;
}
void save_fame(GAME_T* GAME, char* name, int points) {
    qsort(GAME->fame.gracz, MAX_FAME, sizeof(GRACZ_T), asc_fame);
    if (points > GAME->fame.gracz[0].wynik) {
        strcpy(GAME->fame.gracz[0].nazwa, name);
        GAME->fame.gracz[0].wynik = points;
    }
    qsort(GAME->fame.gracz, MAX_FAME, sizeof(GRACZ_T), desc_fame);
    FILE* file = fopen(FAME_PATH, "w");
    for (int i = 0; i < MAX_FAME; i++) {
        if (GAME->fame.gracz[i].nazwa[0] == '\0') break;
        fprintf(file, "%s %d\n", GAME->fame.gracz[i].nazwa,
                GAME->fame.gracz[i].wynik);
    }
    fclose(file);
}

void conclude(GAME_T* GAME) {
    save_game(GAME);
    paint_NAME(GAME->menu.window, GAME);
    char name[MAX_NAME];
    get_name(name, GAME->menu.window, GAME);
    int points = GAME->gracz_A.wynik > GAME->gracz_B.wynik
                     ? GAME->gracz_A.wynik
                     : GAME->gracz_B.wynik;
    save_fame(GAME, name, points);
}

int decide_controls(WINDOW* window, GAME_T* GAME) {
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
            if (GAME->ended) conclude(GAME);
            run(GAME);
            break;
        case 'n':
            w_wprintw("Next");
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
    if (gracz == -1) {
        watrr(A_BOLD, atrrCLR(kolor, mvwprintw(window, 1, 2, "%s", str););
              atrrCLR(ORANGE, mvwprintw(window, 2, 2, "Replay options:")););
    } else {
        watrr(A_BOLD, atrrCLR(kolor, mvwprintw(window, 1, 2, "%s", str););
              atrrCLR(gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B,
                      mvwprintw(window, 2, 2, "Ruch-Gracza %s: ",
                                gracz == PLAYER_A ? "A" : "B")););
    }
    wrefresh(window);
}

int get_number(WINDOW* window, GAME_T* GAME, int gracz) {
    W_GETNSTR_IN((gracz == -1 ? 20 : 2), 2,
                 CONTROLS_PADD + (gracz == -1 ? 0 : 1))
    int result = atoi(in);
    if (gracz == -1) {
        if (!result && in[0] != '0') {
            w_wprintw(TXT_WRONG);
            return get_number(window, GAME, gracz);
        }
        return result;
    }
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

int get_dice(WINDOW* window, GAME_T* GAME, int gracz, int multi, int start) {
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
void move_pionek(GAME_T* GAME, MOVE_T move, int gracz) {
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
    paint_STATE(GAME->aside.window, GAME);
    save_move(GAME, pionek, cel);
}

void capture(GAME_T* GAME, int docelowe, int gracz) {
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_A;
        BAR_PLAYER_B.liczba++;
    } else {
        GAME->plansza.pole[docelowe].kolor = CLR_PLAYER_B;
        BAR_PLAYER_A.liczba++;
    }
    GAME->plansza.pole[docelowe].liczba = 0;
}
int verify_move(GAME_T* GAME, int start, int cel, int gracz, int multi) {
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

int bar_empty(GAME_T* GAME, int gracz) {
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

int asc_KOSTKA(const void* a, const void* b) {
    return (*(KOSTKA_T*)a).value - (*(KOSTKA_T*)b).value;
}

MOVE_T check_A_capture(GAME_T* GAME, int kolor, KOSTKA_T copy[4]) {
    MOVE_T move;
    for (int j = 0; j < POLE_COUNT + 1; j++) {
        for (int i = 0; i < 4; i++) {
            if (copy[i].value < 1) continue;
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int cel = j + copy[i].value;
                if (cel < 1 || cel > 24) continue;
                if (GAME->plansza.pole[cel].kolor != kolor &&
                    GAME->plansza.pole[cel].liczba == 1) {
                    move.kostka = copy[i].index + 1;
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

MOVE_T check_B_capture(GAME_T* GAME, int kolor, KOSTKA_T copy[4]) {
    MOVE_T move;
    for (int j = POLE_COUNT + 1; j > 0; j--) {
        for (int i = 0; i < 4; i++) {
            if (copy[i].value < 1) continue;
            if (GAME->plansza.pole[j].kolor == kolor &&
                GAME->plansza.pole[j].liczba > 0) {
                int docelowe = j - copy[i].value;
                if (docelowe < 1 || docelowe > 24) continue;
                if (GAME->plansza.pole[docelowe].kolor != kolor &&
                    GAME->plansza.pole[docelowe].liczba == 1) {
                    move.kostka = copy[i].index + 1;
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

int capture_possible(GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;

    KOSTKA_T kostki[4];
    for (int i = 0; i < 4; i++) {
        kostki[i].value = GAME->dice[i];
        kostki[i].index = i;
    }
    qsort(kostki, 4, sizeof(KOSTKA_T), asc_KOSTKA);

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

int enforce_move(MOVE_T forced, MOVE_T move, int capture_flag, int gracz,
                 GAME_T* GAME) {
    if (capture_flag) {
        if (move.kostka != forced.kostka) {
            comms(GAME->controls.window, "Wrong dice", RED, gracz);
            pause();
            return 1;
        }
        if (move.pionek != forced.pionek) {
            comms(GAME->controls.window, "Wrong field", RED, gracz);
            pause();
            return 1;
        }
    }
    return 0;
}

int check_A_moves(GAME_T* GAME, int kolor, int kostki[4]) {
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

int check_B_moves(GAME_T* GAME, int kolor, int kostki[4]) {
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int j = POLE_COUNT + 1; j > 0; j--) {
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

int move_possible(GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    int kostki[4] = {GAME->dice[0], GAME->dice[1], GAME->dice[2],
                     GAME->dice[3]};

    if (gracz == PLAYER_A)
        return check_A_moves(GAME, kolor, kostki);
    else
        return check_B_moves(GAME, kolor, kostki);
}

void multi_move(WINDOW* window, GAME_T* GAME, int gracz, int start) {
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

int can_take_home(GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    int start = gracz == PLAYER_A ? 19 : 1;
    int i, suma = 0;
    for (i = start; i < start + 6; i++) {
        if (GAME->plansza.pole[i].kolor == kolor) {
            suma += GAME->plansza.pole[i].liczba;
        }
    }
    if (suma == 15 - (gracz == PLAYER_A ? GAME->plansza.dwor.gracz_A
                                        : GAME->plansza.dwor.gracz_B)) {
        return 1;
    }
    return 0;
}

void move_to_home(GAME_T* GAME, MOVE_T move, int gracz) {
    int* home = gracz == PLAYER_A ? &GAME->plansza.dwor.gracz_A
                                  : &GAME->plansza.dwor.gracz_B;
    int start = move.pionek;
    GAME->plansza.pole[start].liczba--;
    if (GAME->plansza.pole[start].liczba == 0)
        GAME->plansza.pole[start].kolor = 0;
    (*home)++;
    GAME->pozostałe_ruchy--;
    GAME->dice[move.kostka - 1] = 0;
    GAME->home_news++;
    save_move(GAME, start, TRASH_FIELD);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    paint_DICE(GAME->ui_2.window, GAME);
}

int is_better_home_B(GAME_T* GAME, MOVE_T move, int* kostki, int home_start,
                     int gracz) {
    MOVE_T optim;
    int optim_flag = 0;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int pole = home_start; pole < home_start + 6; pole++) {
            if (GAME->plansza.pole[pole].kolor != CLR_PLAYER_B) continue;
            if (pole - kostki[i] == 0) {
                optim.kostka = kostki[i];
                optim.pionek = pole;
                optim_flag = 1;
                if (GAME->dice[move.kostka - 1] == optim.kostka &&
                    move.pionek == optim.pionek) {
                    return 1;
                }
            }
        }
    }
    if (!optim_flag && move.pionek + gracz_step(move.kostka - 1) <= 0) return 1;
    return 0;
}

int is_better_home_A(GAME_T* GAME, MOVE_T move, int* kostki, int home_start,
                     int gracz) {
    MOVE_T optim;
    int optim_flag = 0;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int pole = home_start + 5; pole > home_start - 1; pole--) {
            if (GAME->plansza.pole[pole].kolor != CLR_PLAYER_A) continue;
            if (pole + kostki[i] == 25) {
                optim.kostka = kostki[i];
                optim.pionek = pole;
                optim_flag = 1;
                if (GAME->dice[move.kostka - 1] == optim.kostka &&
                    move.pionek == optim.pionek) {
                    return 1;
                }
            }
        }
    }
    if (!optim_flag && move.pionek + gracz_step(move.kostka - 1) >= 25)
        return 1;
    return 0;
}

int is_better_home(GAME_T* GAME, MOVE_T move, int gracz) {
    int kostki[4] = {GAME->dice[0], GAME->dice[1], GAME->dice[2],
                     GAME->dice[3]};

    qsort(kostki, 4, sizeof(int), asc_dice);

    if (gracz == PLAYER_A) {
        return is_better_home_A(GAME, move, kostki, 19, gracz);
    } else {
        return is_better_home_B(GAME, move, kostki, 1, gracz);
    }
}

void move_action(WINDOW* window, GAME_T* GAME, int gracz) {
    char kostki[50];
    MOVE_T move, forced;
    int bar_flag = 0, capture_flag = 0;
    forced.kostka = -1;
    int home_flag = can_take_home(GAME, gracz);

    if (!move_possible(GAME, gracz) && !home_flag) {
        comms(window, TXT_MOVE_IMP, RED, gracz);
        pause();
        GAME->pozostałe_ruchy = 0;
        return;
    }
    // bar
    if (!bar_empty(GAME, gracz)) {
        bar_flag = 1;
        forced.pionek = gracz == PLAYER_A ? 0 : 25;
        comms(window, TXT_BAR_FULL, RED, gracz);
        pause();
    }
    // end bar

    // dwór
    if (home_flag) {
        comms(window, TXT_HOME_POS, GREEN, gracz);
        pause();
    }

    // end dwór

    // capture
    if (capture_possible(GAME, gracz)) {
        KOSTKA_T kostki[4];
        for (int i = 0; i < 4; i++) {
            kostki[i].value = GAME->dice[i];
            kostki[i].index = i;
        }
        qsort(kostki, 4, sizeof(KOSTKA_T), asc_KOSTKA);

        move = gracz == PLAYER_A ? check_A_capture(GAME, CLR_PLAYER_A, kostki)
                                 : check_B_capture(GAME, CLR_PLAYER_B, kostki);
        if (bar_flag) {
            if (move.pionek == forced.pionek) {
                forced.kostka = move.kostka;
                capture_flag = 1;
                comms(window, TXT_CAP_POS, RED, gracz);
                pause();
            }
        } else {
            forced.kostka = move.kostka;
            forced.pionek = move.pionek;
            capture_flag = 1;
            comms(window, TXT_CAP_POS, RED, gracz);
            pause();
        }
    }
    // end capture

    // manual
    move.kostka = -1;
    do {
        if (bar_flag) {
            move.pionek = gracz == PLAYER_A ? 0 : 25;
        } else {
            comms(window, TXT_M_PION, GREEN, gracz);
            w_mvwprintw(3, 4, TXT_M_FIELD);
            clearLine(3);
            move.pionek = get_number(window, GAME, gracz);
        }
        int multi =
            capture_flag || home_flag || bar_flag ? MULTI_OFF : MULTI_ON;
        comms(window, (multi ? TXT_M_DICE_M : TXT_M_DICE), GREEN, gracz);
        w_mvwprintw(3, 4, TXT_M_DICE_INFO);
        clearLine(3);
        move.kostka = get_dice(window, GAME, gracz, multi, move.pionek);
        if (move.kostka == -10) return;

    } while (enforce_move(forced, move, capture_flag, gracz, GAME));
    // end manual

    // verify

    int cel = move.pionek + gracz_step(move.kostka - 1);
    if (home_flag && (cel > 24 || cel < 1))
        cel = gracz == PLAYER_A ? 25 : 0;
    else if (cel > 24 || cel < 1) {
        comms(window, TXT_VERIFY_4, ORANGE, gracz);
        pause();
        return move_action(window, GAME, gracz);
    }

    if (verify_move(GAME, move.pionek, cel, gracz, MULTI_OFF)) {
        pause();
        return move_action(window, GAME, gracz);
    }

    if (home_flag) {
        if (!is_better_home(GAME, move, gracz)) {
            comms(window, TXT_BETTER_HOME, RED, gracz);
            pause();
            return move_action(window, GAME, gracz);
        }
        if (cel == 25 || cel == 0) {
            move_to_home(GAME, move, gracz);
            return;
        }
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
    pause();
}
int check_win(GAME_T* GAME) {
    int points = 1;
    if (GAME->plansza.dwor.gracz_A == 15) {
        if (GAME->plansza.dwor.gracz_B == 0) {
            points = 2;
            if (BAR_PLAYER_B.liczba > 0) points = 3;
        }
        return points;
    } else if (GAME->plansza.dwor.gracz_B == 15) {
        if (GAME->plansza.dwor.gracz_A == 0) {
            points = 2;
            if (BAR_PLAYER_A.liczba > 0) points = 3;
        }
        return points;
    }
    return 0;
}
int turn(WINDOW* window, GAME_T* GAME, int gracz) {
    GAME->turn++;
    comms(window, GAME->komunikat, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r')
        comms(window, TXT_DECIDE_R, RED, gracz);

    GAME->pozostałe_ruchy = roll(GAME);
    paint_STATE(GAME->aside.window, GAME);
    while (GAME->pozostałe_ruchy > 0) {
        comms(window, TXT_DECIDE_M, GREEN, gracz);
        while (decide_controls(window, GAME) != 'm')
            comms(window, "move", RED, gracz);

        move_action(window, GAME, gracz);
        if (check_win(GAME)) return check_win(GAME);
    }
    comms(window, TXT_DECIDE_S, GREEN, gracz);
    while (decide_controls(window, GAME) != 's')
        comms(window, "skip", RED, gracz);
    return 0;
}
int start_next(GAME_T* GAME, int gracz) {
    WINDOW* window = GAME->controls.window;
    comms(window, TXT_START_NEXT, GREEN, gracz);
    while (decide_controls(window, GAME) != 'n')
        comms(window, TXT_START_NEXT, ORANGE, gracz);
    return 1;
}
void gameplay(GAME_T* GAME, int gracz) {
    paint_DICE(GAME->ui_2.window, GAME);
    int win = 0;
    while (1) {
        if (gracz == PLAYER_A) {
            win = turn(GAME->controls.window, GAME, PLAYER_A);
            save_turn(GAME, GAME->ruchy, 'A');
            if (win) break;
            win = turn(GAME->controls.window, GAME, PLAYER_B);
            save_turn(GAME, GAME->ruchy, 'B');
            if (win) break;
        } else {
            win = turn(GAME->controls.window, GAME, PLAYER_B);
            save_turn(GAME, GAME->ruchy, 'B');
            if (win) break;
            win = turn(GAME->controls.window, GAME, PLAYER_A);
            save_turn(GAME, GAME->ruchy, 'A');
            if (win) break;
        }
    }
    if (win) exec_win(GAME, win, gracz);
}

void paint_GAMEVIEW(GAME_T* GAME) {
    paint_STATE(GAME->aside.window, GAME);
    paint_DICE(GAME->ui_2.window, GAME);
    paint_CONTROLS(GAME->controls.window, GAME, PLAYER_A);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
}
void starting_roll(GAME_T* GAME, WINDOW* window, int gracz) {
    comms(window, TXT_START_ROLL, GREEN, gracz);
    wrefresh(window);
    while (decide_controls(window, GAME) != 'r') {
        comms(window, TXT_START_ROLL, RED, gracz);
    }
    GAME->dice[gracz - 1] = rand() % 6 + 1;
    paint_DICE(GAME->ui_2.window, GAME);
    wrefresh(window);
}
int who_starts(GAME_T* GAME) {
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

void crud_move_pionek(GAME_T* GAME, int start, int cel, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    if (GAME->plansza.pole[cel].kolor != 0 &&
        GAME->plansza.pole[cel].kolor != kolor) {
        capture(GAME, cel, gracz);
    }
    GAME->plansza.pole[cel].kolor = kolor;

    GAME->plansza.pole[cel].liczba++;
    GAME->plansza.pole[start].liczba--;
    GAME->plansza.pole[TRASH_FIELD].kolor = 0;
    GAME->plansza.pole[TRASH_FIELD].liczba = 0;
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
}

int load_save(GAME_T* GAME, FILE* file, int recur, int limit) {
    initGame(GAME);
    int lines = 0;
    char buffer[2][3], c;
    if (!recur) fscanf(file, "%*c");
    fscanf(file, "EED: %d | SCORE: A%d B%d", &GAME->rand_seed,
           &GAME->gracz_A.wynik, &GAME->gracz_B.wynik);
    move(0, strlen(TXT_AUTHOR));
    printw(" | SEED: %d", GAME->rand_seed);
    refresh();
    int gracz = 0, home;
    while (fscanf(file, "%c", &c) != EOF && (lines < limit - 1 || limit < 0)) {
        if (c == '\n') {
            lines++;
            fscanf(file, "->%d%c %d", &GAME->turn, &c, &home);
            if (c == 'A') {
                gracz = PLAYER_A;
                GAME->plansza.dwor.gracz_A += home;
            } else if (c == 'B') {
                gracz = PLAYER_B;
                GAME->plansza.dwor.gracz_B += home;
            }
        }
        if (c == 'm') {
            int pionek, cel;
            fscanf(file, "%s %s", buffer[0], buffer[1]);
            pionek = atoi(buffer[0]);
            cel = atoi(buffer[1]);
            crud_move_pionek(GAME, pionek, cel, gracz);
        }
        if (c == 'S') {
            return load_save(GAME, file, 1, limit - lines);
        }
    }
    return gracz == PLAYER_A ? PLAYER_B : PLAYER_A;
}
void exec_win(GAME_T* GAME, int points, int gracz) {
    GAME->gracz_A.wynik += gracz == PLAYER_A ? points : 0;
    GAME->gracz_B.wynik += gracz == PLAYER_B ? points : 0;
    paint_STATE(GAME->aside.window, GAME);
    GAME->ended = 1;
    char buffer[50];
    sprintf(buffer, "Player %s won! [ANY]", gracz == PLAYER_A ? "A" : "B");
    comms(GAME->controls.window, buffer, ORANGE, gracz);
    pause();
    if (start_next(GAME, gracz)) {
        initGame(GAME);
        paint_GAMEVIEW(GAME);
        comms(GAME->controls.window, "NEW GAME [ANY]", GREEN, gracz);
        pause();
        int gracz = who_starts(GAME);
        gracz = PLAYER_A;
        initGame(GAME);
        save_game(GAME);
        gameplay(GAME, gracz);
    }
}

int decide_replay(WINDOW* window, GAME_T* GAME) {
    w_mvwprintw(getmaxy(window) / 2, 4, TXT_REPLAY);
    clearLine(getmaxy(window) / 2);
    W_GETNSTR_IN(1, 2, CONTROLS_PADD + 1);
    int result;
    switch (tolower(in[0])) {
        case 'n':
            w_wprintw("Next");
            result = 1;
            break;
        case 'p':
            w_wprintw("Prev");
            result = 2;
            break;
        case 'g':
            w_wprintw("Go to");
            result = 3;
            break;
        case 's':
            w_wprintw("Start Playing");
            result = 4;
            break;
        case 'e':
            run(GAME);
            break;

        default:
            w_wprintw(TXT_WRONG);
            return decide_replay(window, GAME);
            break;
    }
    char ch = wgetch(window);
    if (ch != '\n') return decide_replay(window, GAME);
    clearLine(2);
    return result;
}

void replay(GAME_T* GAME, FILE* file, int curr, int limit) {
    curr < 0 ? curr = limit : curr;
    curr > limit ? curr = 0 : curr;
    rewind(file);
    int g = load_save(GAME, file, 0, curr);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    paint_STATE(GAME->aside.window, GAME);
    WINDOW* window = GAME->controls.window;
    char buffer[50];
    sprintf(buffer, TXT_REPLAY_1, curr, limit);
    comms(window, buffer, GREEN, -1);
    switch (decide_replay(window, GAME)) {
        case 1:
            curr++;
            break;
        case 2:
            curr--;
            break;
        case 3:
            comms(window, TXT_REPLAY_2, BLUE, -1);
            curr = get_number(window, GAME, -1);
            sprintf(buffer, TXT_REPLAY_3, curr);
            comms(window, buffer, CYAN, -1);
            pause();
            clearLine(3);
            w_mvwprintw(2, getmaxx(window) - 1, "│");
            break;
        case 4:
            gameplay(GAME, g);
            return;

        default:
            break;
    }
    wrefresh(window);
    replay(GAME, file, curr, limit);
}
int count_lines(FILE* file) {
    int lines = 1;
    char c;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == '\n') lines++;
    }
    rewind(file);
    return lines;
}
void run(GAME_T* GAME) {
    int gracz;
    paint_FAME(GAME->aside.window, GAME);
    paint_MENU(GAME->menu.window, GAME);

    refresh();

    switch (decide_menu(GAME->menu.window, GAME)) {
        case 1:
            clear_save();
            initGame(GAME);
            paint_GAMEVIEW(GAME);
            // gracz = PLAYER_A;
            gracz = PLAYER_B;
            // gracz = who_starts(GAME);
            initGame(GAME);
            save_game(GAME);
            gameplay(GAME, gracz);
            break;
        case 2:
            FILE* file2 = fopen(SAVE_PATH, "r+");
            gracz = load_save(GAME, file2, 0, -1);
            fclose(file2);
            paint_GAMEVIEW(GAME);
            gameplay(GAME, gracz);
            break;
        case 3:
            paint_GAMEVIEW(GAME);
            FILE* file3 = fopen(SAVE_PATH, "r+");
            int limit = count_lines(file3);
            comms(GAME->controls.window, TXT_REPLAY_0, GREEN, -1);
            pause();
            replay(GAME, file3, 0, limit);  // 0 -> start
            fclose(file3);
            break;

        default:
            break;
    }
}

void pioneczki(GAME_T* GAME, PIONKI_T pionki[POLE_COUNT]) {
    for (int i = 0; i < POLE_COUNT; i++) {
        if (pionki[i].index == -1) break;
        GAME->plansza.pole[pionki[i].index].liczba = pionki[i].liczba;
        GAME->plansza.pole[pionki[i].index].kolor = pionki[i].kolor;
    }
}

void placePionki(GAME_T* GAME) {
    PIONKI_T pionki[POLE_COUNT];

    FILE* file = fopen(TEMPLATE_PATH, "r");
    fscanf(file, "Dwór: A %d B %d\n", &GAME->plansza.dwor.gracz_A,
           &GAME->plansza.dwor.gracz_B);
    fscanf(file, "Banda: A %d B %d\n\n", &BAR_PLAYER_A.liczba,
           &BAR_PLAYER_B.liczba);
    char c;
    for (int i = 0; i < POLE_COUNT; i++) {
        if (fscanf(file, "%d %d %c\n", &pionki[i].index, &pionki[i].liczba,
                   &c) == EOF) {
            pionki[i].index = -1;
            continue;
        }
        pionki[i].kolor = c == 'A' ? CLR_PLAYER_A : CLR_PLAYER_B;
    }

    fclose(file);

    pioneczki(GAME, pionki);
}
void RESET_GAME(GAME_T* GAME) {
    GAME->ended = 0;
    GAME->pozostałe_ruchy = 0;
    GAME->turn = 0;
    GAME->home_news = 0;

    BAR_PLAYER_A.liczba = 2;
    BAR_PLAYER_B.liczba = 2;
    BAR_PLAYER_A.kolor = CLR_PLAYER_A;
    BAR_PLAYER_B.kolor = CLR_PLAYER_B;

    GAME->plansza.dwor.gracz_A = 12;
    GAME->plansza.dwor.gracz_B = 0;

    GAME->dice[0] = -1;
    GAME->dice[1] = -1;
    GAME->dice[2] = -1;
    GAME->dice[3] = -1;
}
void initGame(GAME_T* GAME) {
    GAME->rand_seed = time(NULL);
    srand(GAME->rand_seed);
    move(0, strlen(TXT_AUTHOR));
    printw(" | SEED: %d", GAME->rand_seed);
    refresh();

    for (int i = 1; i < POLE_COUNT + 1; i++) {
        GAME->plansza.pole[i].liczba = 0;
        GAME->plansza.pole[i].kolor = 0;
        GAME->plansza.pole[i].number = i;
    }
    RESET_GAME(GAME);

    placePionki(GAME);

    strcpy(GAME->komunikat, TXT_TURN);
}

int main() {
    GAME_T* GAME = (GAME_T*)malloc(sizeof(GAME_T));
    if (GAME == NULL) return 1;
    if (initialInit(GAME)) {
        printw(TXT_AUTHOR);
        GAME->gracz_A.wynik = 0;
        GAME->gracz_B.wynik = 0;
        initWindows(GAME);
        initGame(GAME);
        run(GAME);
    }
    pause();
    free(GAME);
    endwin();
    return 0;
}
