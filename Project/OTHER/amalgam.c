#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define.h"
#include "text.h"
// misc.c
void colorInit();
char initialInit();
void initWindows(GAME_T* GAME);
int asc_fame(const void* a, const void* b);
int desc_fame(const void* a, const void* b);
int asc_dice(const void* a, const void* b);
int desc_dice(const void* a, const void* b);
int asc_KOSTKA(const void* a, const void* b);

// paint.c
void paint_MENU(WINDOW* window, GAME_T* GAME);
void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window, POLE_T pole);
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window, POLE_T pole);
void paint_BAR_A(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR_B(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_BAR(int x, int y, WINDOW* window, GAME_T* GAME);
void paint_DWOR(int x, int y, WINDOW* window, DWOR_T dwor);
void paint_BOARD(WINDOW* window, GAME_T* GAME, int pad);
void paint_GAMEVIEW(GAME_T* GAME);
void paint_STATE(WINDOW* window, GAME_T* GAME);
void paint_FAME(WINDOW* window, GAME_T* GAME);
void paint_CONTROLS(WINDOW* window, GAME_T* GAME, int gracz);
void printDICE(WINDOW* win, int y, int x, GAME_T* GAME, int index);
void paint_DICE(WINDOW* window, GAME_T* GAME);
void paint_NAME(WINDOW* window, GAME_T* GAME);

// index.c
void crud_move_pionek(GAME_T* GAME, int start, int cel, int gracz);
void multi_move(WINDOW* window, GAME_T* GAME, int gracz, int start);
void exec_win(GAME_T* GAME, int points);
void capture(GAME_T* GAME, int docelowe, int gracz);
void initGame(GAME_T* GAME);
void run(GAME_T* GAME);

// Stałe
#define TRUE 1
#define FALSE 0
#define MAX_FAME 9
#define MAX_NAME 20
#define MAX_BAR 6
#define POLE_COUNT 24
#define MAX_LONG_STR 200
#define MAX_SHORT_STR 20

#define PLAYER_A 1
#define PLAYER_B 2

#define MULTI_ON 1
#define MULTI_OFF 0

#define TRASH_FIELD 26

#define BAR_PLAYER_A GAME->plansza.pole[0]
#define BAR_PLAYER_B GAME->plansza.pole[25]

#define CLR_PLAYER_A CYAN
#define CLR_PLAYER_B MAGENTA

#define STARTED 0
#define PLAYING 1
#define ENDED 2

#define PIONKI_HEIGHT 5
#define BOARD_PADDING 2
#define BOARD_WIDTH 47
#define PADDED_BOARD_WIDTH (BOARD_WIDTH + (BOARD_PADDING * 3))
#define PADDED_BOARD_HEIGHT (BOARD_HEIGHT + (BOARD_PADDING * 2))

#define CTRLS_PADD 15 + 2  // strlen("Ruch-Gracza X: ") + x
#define MENU_PADD 18 + 3   // strlen("Choose from list: ") + x

#define BOARD_HEIGHT 13

// FILE PATHS
#define FAME_PATH "ARCHIVE/fame.txt"
#define SAVE_PATH "ARCHIVE/save.txt"
#define TEMPLATE_PATH "ARCHIVE/template.txt"
#define TEMP_PATH "ARCHIVE/temp.txt"

// Znaki
// #define BLOCK "\u25A7"
#define BLOCK "\u25A0"
#define PIONEK "\u25CF"
#define POLE_UP "\u25B3"
#define POLE_DOWN "\u25BD"
#define HR "\u2500"
// #define LINE "\u2507"
#define LINE "|"

// Kolory
#define COLOR_DEFAULT 0
#define BLUE_YELLOW 1
#define RED 2
#define YELLOW 3
#define GREEN 4
#define BLUE 5
#define BROWN 6
#define DARK_BLUE 7
#define MAGENTA 8
#define CYAN 9
#define ORANGE 10

#define COLOR_BROWN 80
#define COLOR_DARK_BLUE 81
#define COLOR_ORANGE 82

// Makra
#define W_GETNSTR_IN(n, y, x) \
    char in[n + 1];           \
    wmove(window, y, x);      \
    wgetnstr(window, in, n);  \
    wmove(window, y, x);

#define CLR(x) COLOR_PAIR(x)

#define atrrCLR(clr, _print)          \
    wattron(window, COLOR_PAIR(clr)); \
    _print;                           \
    wattroff(window, COLOR_PAIR(clr));

#define watrr(other, _print) \
    wattron(window, other);  \
    _print;                  \
    wattroff(window, other);

#define atrr(other, _print) \
    attron(other);          \
    _print;                 \
    attroff(other);

#define w_mvwprintw(y, x, str) mvwprintw(window, y, x, str);

#define w_wprintw(str) wprintw(window, str);

#define clearLine(y)                          \
    wclrtoeol(window);                        \
    w_mvwprintw(y, getmaxx(window) - 1, "│"); \
    w_mvwprintw(y, 0, "│");                   \
    wrefresh(window);

#define pause() \
    noecho();   \
    getch();    \
    echo();

#define gracz_step(index) \
    (gracz == PLAYER_A ? GAME->dice[index] : -GAME->dice[index])

#define LOG(str, ...)                   \
    FILE* file = fopen("log.txt", "a"); \
    fprintf(file, str, ##__VA_ARGS__);  \
    fclose(file);

// Struktury
typedef struct {
    int liczba;  // 0 - brak, 1-15 - ilość pionków
    int kolor;   // 0 - brak, 1 - gracz A, 2 - gracz B
    int number;  // number of field
} POLE_T;
typedef struct {
    int gracz_A;  // 0 - brak, 1-15 - ilość pionków
    int gracz_B;  // 0 - brak, 1-15 - ilość pionków
} DWOR_T;
typedef struct {
    WINDOW* window;
    POLE_T pole[POLE_COUNT + 3];  // 0 - bar A, 25 - bar B, 26 - trash
    DWOR_T dwor;
} PLANSZA_T;
typedef struct {
    char nazwa[MAX_NAME];
    int wynik;
} GRACZ_T;
typedef struct {
    GRACZ_T gracz[MAX_FAME];
} FAME_T;
typedef struct {
    WINDOW* window;
} UI_T;

typedef struct {
    PLANSZA_T plansza;
    GRACZ_T gracz_A;
    GRACZ_T gracz_B;
    FAME_T fame;
    UI_T aside;
    UI_T controls;
    UI_T ui_2;
    UI_T menu;
    int dice[4];
    char komunikat[MAX_LONG_STR];
    int rand_seed;
    int dublet;
    int leftMoves;
    char ruchy[MAX_LONG_STR];
    int home_news;
    int turn;
    int ended;
    int winner;
    int bar_f;
} GAME_T;

typedef struct {
    int kostka;
    int pionek;
} MOVE_T;

typedef struct {
    int index;
    int value;
} KOSTKA_T;

typedef struct {
    int index, liczba, kolor;
} PIONKI_T;

#define TXT_VER_1 "You have to choose your field!"
#define TXT_VER_2 "You have to choose field with pieces!"
#define TXT_VER_3 "You have to choose conquerable field!"
#define TXT_VER_4 "You can't move to home"
#define TXT_CAPTURE "Ładne bicie"
#define TXT_MOVE_IMP "You have to  skip (no legal moves)! [ANY]"
#define TXT_BAR_FULL "You have to  move all pieces from bar! (ANY)"
#define TXT_CAP_POS "You have to capture! [ANY]"

#define TXT_M_PION "Choose field with your pieces"
#define TXT_M_FIELD "Choose number from 1 to 24"
#define TXT_M_DICE "Choose dice"
#define TXT_M_DICE_M "Choose dice or [m] for multi dice"
#define TXT_M_DICE_N "Choose next dice (or 0 to end turn)"
#define TXT_M_DICE_INFO "Choose number of dice from bottom right panel"

#define TXT_POST_MOVE "Ruszyłeś pionka z pola %d na pole %d"
#define TXT_POST_MOVE_BAR "Ruszyłeś pionka z bandy na pole %d"

#define TXT_DECIDE_R "You have to roll dice first!"
#define TXT_DECIDE_M "You can move now"
#define TXT_DECIDE_S "You can skip now"

#define TXT_START_ROLL "Roll to decide who starts"
#define TXT_START_PLAYER_A "Player A starts [ANY]"
#define TXT_START_PLAYER_B "Player B starts [ANY]"

#define TXT_TURN "Good luck!"

#define TXT_AUTHOR "Wojciech Siwiec | Indeks: s197815 | Rok: 2023/24"
#define TXT_GAME_NAME "Backgammon─1.0"

#define TXT_DICE_ROLL "Throw dice!"
#define TXT_DICE_USED "Dice already used!"

#define TXT_DICE_NR "Dice Nr %d"

#define TXT_WRONG "Wrong input!"

#define TXT_CLR_UNSUPPORTED "Terminal doesn't support colors!"

#define TXT_CONTROLS "R(oll) | M(ove) | S(kip) | E(xit)"

#define TXT_HOME_POS "You can move to court! [ANY]"

#define TXT_BETTER_HOME "You have to choose optimal move!"

#define TXT_START_NEXT "Enter [n] to start next game or [e] to exit"
#define TXT_DECIDE_N "You can move now"

#define TXT_CONTROLS "R(oll) | M(ove) | S(kip) | E(xit)"
#define TXT_REPLAY "N(ext) | P(revious) | (G)oto | (S)tart | E(xit)"

#define TXT_REPLAY_0 "Welcome to replay mode!"
#define TXT_REPLAY_1 "Current step: %d | Max steps: %d"
#define TXT_REPLAY_2 "Choose step to go to"
#define TXT_REPLAY_3 "Going to step: %d"

#define TXT_NAME "Enter your name:"

#define TXT_MUST_MOVE "You have to move!"
#define TXT_MUST_SKIP "You have to skip!"

#define TXT_M_MOVE "CHOOSE DICES [np.: 2+1+3]"
#define TXT_M_VER_1 "WRONG DICE NUMBERS"
#define TXT_M_VER_2 "DICE ALREADY USED"
#define TXT_M_VER_3 "DICE USED TWICE"

#include "headers.h"

int asc_fame(const void* a, const void* b) {
    return ((GRACZ_T*)a)->wynik - ((GRACZ_T*)b)->wynik;
}
int desc_fame(const void* a, const void* b) {
    return ((GRACZ_T*)b)->wynik - ((GRACZ_T*)a)->wynik;
}
int asc_dice(const void* a, const void* b) { return (*(int*)a - *(int*)b); }
int desc_dice(const void* a, const void* b) { return (*(int*)b - *(int*)a); }

int asc_KOSTKA(const void* a, const void* b) {
    return (*(KOSTKA_T*)a).value - (*(KOSTKA_T*)b).value;
}

void colorInit() {
    init_pair(1, COLOR_BLUE, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_color(COLOR_BROWN, 500, 250, 0);
    init_color(COLOR_DARK_BLUE, 0, 0, 500);
    init_pair(6, COLOR_BROWN, COLOR_BLACK);
    init_pair(7, COLOR_DARK_BLUE, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(9, COLOR_CYAN, COLOR_BLACK);
    init_color(COLOR_ORANGE, 1000, 500, 0);
    init_pair(10, COLOR_ORANGE, COLOR_BLACK);
}

void initWindows(GAME_T* GAME) {
    GAME->plansza.window =
        newwin(PADDED_BOARD_HEIGHT, PADDED_BOARD_WIDTH, 2, 4);

    GAME->controls.window =
        newwin(PADDED_BOARD_HEIGHT / 3 + 1, PADDED_BOARD_WIDTH,
               PADDED_BOARD_HEIGHT + 1, 4);

    GAME->ui_2.window =
        newwin(PADDED_BOARD_HEIGHT / 3 + 1, PADDED_BOARD_WIDTH / 2,
               PADDED_BOARD_HEIGHT + 1, PADDED_BOARD_WIDTH + 4);

    GAME->menu.window = newwin(PADDED_BOARD_HEIGHT + PADDED_BOARD_HEIGHT / 3,
                               PADDED_BOARD_WIDTH, 2, 4);

    GAME->aside.window =
        newwin(PADDED_BOARD_HEIGHT + PADDED_BOARD_HEIGHT / 3,
               PADDED_BOARD_WIDTH / 2, 2, PADDED_BOARD_WIDTH + 4);

    refresh();
}

char initialInit() {
    setlocale(LC_ALL, "");
    initscr();
    if (has_colors() == TRUE) {
        start_color();
        colorInit();
    } else {
        printw(TXT_CLR_UNSUPPORTED);
        return FALSE;
    }
    return TRUE;
}

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
    if (tmp > 0)
        mvwprintw(window, y + i, x - 1, "+%d", tmp);
    else
        mvwprintw(window, y + i, x - 1, "  ");
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
    if (tmp > 0)
        mvwprintw(window, y - 1, x - 1, "+%d", tmp);
    else
        mvwprintw(window, y - 1, x - 1, "  ");
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

#include "Partials/headers.h"
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
    GAME->dice[0] = rand() % 6 + 1;
    GAME->dice[1] = rand() % 6 + 1;
    // GAME->dice[0] = 1;
    // GAME->dice[1] = 1;

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

void get_name(char* name, WINDOW* window, GAME_T* GAME) {
    W_GETNSTR_IN(MAX_NAME, 1, strlen(TXT_NAME) + 2);
    strcpy(name, in);
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

int decide_ctrls(WINDOW* window, GAME_T* GAME) {
    w_mvwprintw(getmaxy(window) / 2, 4, TXT_CONTROLS);
    clearLine(getmaxy(window) / 2);
    W_GETNSTR_IN(1, 2, CTRLS_PADD);
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
            return decide_ctrls(window, GAME);
            break;
    }
    char ch = wgetch(window);
    if (ch != '\n') return decide_ctrls(window, GAME);
    clearLine(2);
    return tolower(in[0]);
}

void info(WINDOW* window, char* str, int kolor, int gracz) {
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
    W_GETNSTR_IN((gracz == -1 ? 20 : 2), 2, CTRLS_PADD + (gracz == -1 ? 0 : 1))
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
    wmove(window, 2, CTRLS_PADD);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_number(window, GAME, gracz);
    clearLine(2);
    return result;
}

int get_dice(GAME_T* GAME, int gracz, int multi, int start) {
    WINDOW* window = GAME->controls.window;
    W_GETNSTR_IN(1, 2, CTRLS_PADD)

    if (in[0] == 'm' && multi) {
        multi_move(window, GAME, gracz, start);
        return -10;
    }
    int out = atoi(in);
    if (out < 1 || out > (GAME->dice[3] != -1 ? 4 : 2)) {
        w_wprintw(TXT_WRONG);
        clearLine(2);
        return get_dice(GAME, gracz, multi, start);
    }
    if (GAME->dice[out - 1] == 0) {
        w_wprintw(TXT_DICE_USED);
        return get_dice(GAME, gracz, multi, start);
    }

    wprintw(window, TXT_DICE_NR, out);
    wrefresh(window);
    char ch = wgetch(window);
    if (ch != '\n') return get_dice(GAME, gracz, multi, start);
    clearLine(2);
    return out;
}
void move_pionek(GAME_T* GAME, MOVE_T move, int gracz) {
    int pionek = move.pionek, kostka = move.kostka;
    int cel = pionek + gracz_step(kostka - 1);
    POLE_T* p_cel = &(GAME->plansza.pole[cel]);
    if (gracz == PLAYER_A) {
        GAME->plansza.pole[pionek].liczba--;
        p_cel->liczba++;
        p_cel->kolor = CLR_PLAYER_A;
    } else {
        GAME->plansza.pole[pionek].liczba--;
        p_cel->liczba++;
        p_cel->kolor = CLR_PLAYER_B;
    }
    GAME->dice[kostka - 1] = 0;
    paint_DICE(GAME->ui_2.window, GAME);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    GAME->leftMoves--;
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
    int clr = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;
    POLE_T* p_cel = &(GAME->plansza.pole[cel]);
    if (GAME->plansza.pole[start].kolor != clr) {
        info(win, TXT_VER_1, RED, gracz);
        return 1;
    }
    if (GAME->plansza.pole[start].liczba == 0) {
        info(win, TXT_VER_2, RED, gracz);
        return 1;
    }
    if (p_cel->kolor != clr && p_cel->liczba > 1) {
        info(win, TXT_VER_3, RED, gracz);
        return 1;
    }
    if (p_cel->kolor != clr && p_cel->liczba == 1) {
        if (multi == MULTI_OFF) {
            capture(GAME, cel, gracz);
            info(win, TXT_CAPTURE, GREEN, gracz);
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

MOVE_T checkAcap(GAME_T* GAME, int kolor, KOSTKA_T copy[4]) {
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

MOVE_T checkBcap(GAME_T* GAME, int kolor, KOSTKA_T copy[4]) {
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

int cap_poss(GAME_T* GAME, int gracz) {
    int kolor = gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B;

    KOSTKA_T kostki[4];
    for (int i = 0; i < 4; i++) {
        kostki[i].value = GAME->dice[i];
        kostki[i].index = i;
    }
    qsort(kostki, 4, sizeof(KOSTKA_T), asc_KOSTKA);

    if (gracz == PLAYER_A) {
        if (checkAcap(GAME, kolor, kostki).kostka != -1) {
            return 1;
        }
        return 0;

    } else {
        if (checkBcap(GAME, kolor, kostki).kostka != -1) {
            return 1;
        }
        return 0;
    }
}

int enforce_move(MOVE_T forced, MOVE_T move, int cap_f, int gracz,
                 GAME_T* GAME) {
    if (cap_f) {
        if (move.kostka != forced.kostka) {
            info(GAME->controls.window, "Wrong dice", RED, gracz);
            pause();
            return 1;
        }
        if (move.pionek != forced.pionek) {
            info(GAME->controls.window, "Wrong field", RED, gracz);
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
            if (GAME->bar_f && j != 0) continue;
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
            if (GAME->bar_f && j != 25) continue;
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
    GAME->bar_f = !bar_empty(GAME, gracz);
    if (gracz == PLAYER_A)
        return check_A_moves(GAME, kolor, kostki);
    else
        return check_B_moves(GAME, kolor, kostki);
}

void m_mov_m1(WINDOW* window, GAME_T* GAME, int gracz, int start, int* kostki,
              int* ret_f, int i) {
    if (GAME->plansza.pole[start].liczba == 0) {
        w_wprintw(TXT_VER_2);
        *ret_f = 1;
    }
    if (GAME->plansza.pole[start].kolor !=
        (gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B)) {
        w_wprintw(TXT_VER_1);
        *ret_f = 1;
    }
    if (kostki[i] < 1 || kostki[i] > (GAME->dublet ? 4 : 2)) {
        w_wprintw(TXT_M_VER_1);
        *ret_f = 1;
    }
    if (GAME->dice[kostki[i] - 1] == 0) {
        w_wprintw(TXT_M_VER_2);
        *ret_f = 1;
    }
    for (int j = 0; j < 4; j++) {
        if (i == j) continue;
        if (kostki[i] == kostki[j]) {
            w_wprintw(TXT_M_VER_3);
            *ret_f = 1;
            break;
        }
    }
}

void m_mov_m2(WINDOW* window, GAME_T* GAME, int gracz, int start, int* kostki,
              int cel) {
    for (int i = 0, cel = start; i < 4; i++) {
        if (!kostki[i]) continue;
        int prev = cel;
        cel += gracz_step(kostki[i] - 1);
        verify_move(GAME, start, cel, gracz, MULTI_ON);
        GAME->dice[kostki[i] - 1] = 0;
        GAME->leftMoves--;
        save_move(GAME, prev, cel);
    }
    paint_DICE(GAME->ui_2.window, GAME);
    if (GAME->plansza.pole[start].kolor ==
        (gracz == PLAYER_A ? CLR_PLAYER_A : CLR_PLAYER_B)) {
        crud_move_pionek(GAME, start, cel, gracz);
    }
}

void multi_move(WINDOW* win, GAME_T* GAME, int gracz, int st) {
    info(win, TXT_M_MOVE, GREEN, gracz);
    WINDOW* window = win;
    W_GETNSTR_IN(7, 2, CTRLS_PADD);
    int i = 0, ret_f = 0, cel, k[4] = {0};
    char* split = strtok(in, "+");
    while (split != NULL) {
        k[i++] = atoi(split);
        split = strtok(NULL, "+");
    }
    for (i = 0; i < 4; i++) {
        if (!k[i]) continue;
        m_mov_m1(win, GAME, gracz, st, k, &ret_f, i);
        if (ret_f) return multi_move(win, GAME, gracz, st);
    }
    for (i = 0, cel = st; i < 4; i++) {
        if (!k[i]) continue;
        cel += gracz_step(k[i] - 1);
        if (verify_move(GAME, st, cel, gracz, MULTI_OFF)) {
            pause();
            return multi_move(win, GAME, gracz, st);
        }
    }
    m_mov_m2(win, GAME, gracz, st, k, cel);
}

int court_poss(GAME_T* GAME, int gracz) {
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
    GAME->leftMoves--;
    GAME->dice[move.kostka - 1] = 0;
    GAME->home_news++;
    save_move(GAME, start, TRASH_FIELD);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    paint_DICE(GAME->ui_2.window, GAME);
}

int less_optimal_B(GAME_T* GAME, MOVE_T move, int* kostki, int home_start,
                   int gracz) {
    MOVE_T optim;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int pole = home_start + 5; pole > home_start - 1; pole--) {
            if (GAME->plansza.pole[pole].kolor != CLR_PLAYER_B) continue;
            if (pole - kostki[i] < 0) {
                optim.kostka = kostki[i];
                optim.pionek = pole;
                if (GAME->dice[move.kostka - 1] == optim.kostka &&
                    move.pionek == optim.pionek) {
                    return 1;
                } else
                    return 0;
            }
        }
    }
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
    if (!optim_flag && less_optimal_B(GAME, move, kostki, home_start, gracz))
        return 1;
    return 0;
}

int less_optimal_A(GAME_T* GAME, MOVE_T move, int* kostki, int home_start,
                   int gracz) {
    MOVE_T optim;
    for (int i = 0; i < 4; i++) {
        if (kostki[i] < 1) continue;
        for (int pole = home_start; pole < home_start + 6; pole++) {
            if (GAME->plansza.pole[pole].kolor != CLR_PLAYER_A) continue;
            if (pole + kostki[i] > 25) {
                optim.kostka = kostki[i];
                optim.pionek = pole;
                if (GAME->dice[move.kostka - 1] == optim.kostka &&
                    move.pionek == optim.pionek) {
                    return 1;
                } else
                    return 0;
            }
        }
    }
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
    if (!optim_flag && less_optimal_A(GAME, move, kostki, home_start, gracz))
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
int other(GAME_T* GAME, int gracz, int* forced_pionek, int* bar_f, int home_f) {
    if (!move_possible(GAME, gracz) && !home_f) {
        info(GAME->controls.window, TXT_MOVE_IMP, RED, gracz);
        pause();
        GAME->leftMoves = 0;
        return 1;
    }
    if (!bar_empty(GAME, gracz)) {
        *bar_f = 1;
        *forced_pionek = gracz == PLAYER_A ? 0 : 25;
        info(GAME->controls.window, TXT_BAR_FULL, RED, gracz);
        pause();
    }
    if (home_f) {
        info(GAME->controls.window, TXT_HOME_POS, GREEN, gracz);
        pause();
    }
    return 0;
}
void capture_module(GAME_T* GM, int g, MOVE_T* m, MOVE_T* f, int* cap_f,
                    int bar_f) {
    if (cap_poss(GM, g)) {
        KOSTKA_T dice[4];
        for (int i = 0; i < 4; i++) {
            dice[i].value = GM->dice[i];
            dice[i].index = i;
        }
        qsort(dice, 4, sizeof(KOSTKA_T), asc_KOSTKA);

        (*m) = g == PLAYER_A ? checkAcap(GM, CLR_PLAYER_A, dice)
                             : checkBcap(GM, CLR_PLAYER_B, dice);
        if (bar_f) {
            if ((*m).pionek == (*f).pionek) {
                (*f).kostka = (*m).kostka;
                *cap_f = 1;
                info(GM->controls.window, TXT_CAP_POS, RED, g);
                pause();
            }
        } else {
            (*f).kostka = (*m).kostka;
            (*f).pionek = (*m).pionek;
            *cap_f = 1;
            info(GM->controls.window, TXT_CAP_POS, RED, g);
            pause();
        }
    }
    (*m).kostka = -1;
}
void enforce_module(GAME_T* GAME, MOVE_T* move, int cap_f, int gracz, int bar_f,
                    int home_f) {
    WINDOW* window = GAME->controls.window;
    if (bar_f) {
        (*move).pionek = gracz == PLAYER_A ? 0 : 25;
    } else {
        info(window, TXT_M_PION, GREEN, gracz);
        w_mvwprintw(3, 4, TXT_M_FIELD);
        clearLine(3);
        (*move).pionek = get_number(window, GAME, gracz);
    }
    int multi = cap_f || home_f || bar_f ? MULTI_OFF : MULTI_ON;
    info(window, (multi ? TXT_M_DICE_M : TXT_M_DICE), GREEN, gracz);
    w_mvwprintw(3, 4, TXT_M_DICE_INFO);
    clearLine(3);
    (*move).kostka = get_dice(GAME, gracz, multi, (*move).pionek);
}
void move_comm(GAME_T* GAME, MOVE_T move, int gracz) {
    WINDOW* window = GAME->controls.window;
    char buffer[100];
    if (move.pionek == 0 | move.pionek == 25) {
        sprintf(buffer, TXT_POST_MOVE_BAR,
                move.pionek + gracz_step(move.kostka - 1));
    } else {
        sprintf(buffer, TXT_POST_MOVE, move.pionek,
                move.pionek + gracz_step(move.kostka - 1));
    }
    info(window, buffer, GREEN, gracz);
}
int verify_module(GAME_T* GAME, MOVE_T move, int gracz, int home_f) {
    int cel = move.pionek + gracz_step(move.kostka - 1);
    WINDOW* win = GAME->controls.window;
    if (home_f && (cel > 24 || cel < 1))
        cel = TRASH_FIELD;
    else if (cel > 24 || cel < 1) {
        info(win, TXT_VER_4, ORANGE, gracz);
        pause();
        return 2;
    }

    if (verify_move(GAME, move.pionek, cel, gracz, MULTI_OFF)) {
        pause();
        return 2;
    }

    if (home_f) {
        if (!is_better_home(GAME, move, gracz)) {
            info(win, TXT_BETTER_HOME, RED, gracz);
            pause();
            return 2;
        }
        if (cel == TRASH_FIELD) {
            move_to_home(GAME, move, gracz);
            return 1;
        }
    }
    return 0;
}
void move_action(WINDOW* window, GAME_T* GAME, int gracz) {
    MOVE_T move, forced;
    int bar_f = 0, cap_f = 0;
    forced.kostka = -1;
    int home_f = court_poss(GAME, gracz);

    if (other(GAME, gracz, &forced.pionek, &bar_f, home_f)) return;

    capture_module(GAME, gracz, &move, &forced, &cap_f, bar_f);

    do {
        enforce_module(GAME, &move, cap_f, gracz, bar_f, home_f);
        if (move.kostka == -10) return;
    } while (enforce_move(forced, move, cap_f, gracz, GAME));

    int valid = verify_module(GAME, move, gracz, home_f);
    if (valid == 1) return;
    if (valid == 2) return move_action(window, GAME, gracz);

    move_comm(GAME, move, gracz);
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
        GAME->winner = PLAYER_A;
        return points;
    } else if (GAME->plansza.dwor.gracz_B == 15) {
        if (GAME->plansza.dwor.gracz_A == 0) {
            points = 2;
            if (BAR_PLAYER_A.liczba > 0) points = 3;
        }
        GAME->winner = PLAYER_B;
        return points;
    }
    return 0;
}
int turn(WINDOW* win, GAME_T* GAME, int gracz) {
    GAME->turn++;
    info(win, GAME->komunikat, GREEN, gracz);
    wrefresh(win);
    while (decide_ctrls(win, GAME) != 'r') info(win, TXT_DECIDE_R, RED, gracz);

    GAME->leftMoves = roll(GAME);
    paint_STATE(GAME->aside.window, GAME);
    while (GAME->leftMoves > 0) {
        info(win, TXT_DECIDE_M, GREEN, gracz);
        while (decide_ctrls(win, GAME) != 'm')
            info(win, TXT_MUST_MOVE, ORANGE, gracz);

        move_action(win, GAME, gracz);
        if (check_win(GAME)) return check_win(GAME);
    }
    info(win, TXT_DECIDE_S, GREEN, gracz);
    while (decide_ctrls(win, GAME) != 's')
        info(win, TXT_MUST_SKIP, ORANGE, gracz);
    return 0;
}
int start_next(GAME_T* GAME, int gracz) {
    WINDOW* window = GAME->controls.window;
    info(window, TXT_START_NEXT, GREEN, gracz);
    while (decide_ctrls(window, GAME) != 'n')
        info(window, TXT_START_NEXT, ORANGE, gracz);
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
            gracz = PLAYER_B;
            win = turn(GAME->controls.window, GAME, PLAYER_B);
            save_turn(GAME, GAME->ruchy, 'B');
            if (win) break;
            win = turn(GAME->controls.window, GAME, PLAYER_A);
            save_turn(GAME, GAME->ruchy, 'A');
            if (win) break;
        }
    }
    if (win) exec_win(GAME, win);
}
void starting_roll(GAME_T* GAME, WINDOW* window, int gracz) {
    info(window, TXT_START_ROLL, GREEN, gracz);
    wrefresh(window);
    while (decide_ctrls(window, GAME) != 'r') {
        info(window, TXT_START_ROLL, RED, gracz);
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
        info(window, TXT_START_PLAYER_A, GREEN, PLAYER_A);
        pause();
        return PLAYER_A;
    } else if (GAME->dice[0] < GAME->dice[1]) {
        info(window, TXT_START_PLAYER_B, GREEN, PLAYER_B);
        pause();
        return PLAYER_B;
    } else {
        info(window, "Roll again", GREEN, PLAYER_A);
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

void load_module(FILE* file, GAME_T* GAME, char c, char buffer[2][3],
                 int* gracz, int* home, int* lines) {
    if (c == '\n') {
        (*lines)++;
        fscanf(file, "->%d%c %d", &GAME->turn, &c, home);
        if (c == 'A') {
            *gracz = PLAYER_A;
            GAME->plansza.dwor.gracz_A += *home;
        } else if (c == 'B') {
            *gracz = PLAYER_B;
            GAME->plansza.dwor.gracz_B += *home;
        }
    }
    if (c == 'm') {
        int pionek, cel;
        fscanf(file, "%s %s", buffer[0], buffer[1]);
        pionek = atoi(buffer[0]);
        cel = atoi(buffer[1]);
        crud_move_pionek(GAME, pionek, cel, *gracz);
    }
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
        load_module(file, GAME, c, buffer, &gracz, &home, &lines);

        if (c == 'S') {
            return load_save(GAME, file, 1, limit - lines);
        }
    }
    return gracz == PLAYER_A ? PLAYER_B : PLAYER_A;
}
void exec_win(GAME_T* GAME, int points) {
    int gracz = GAME->winner;
    GAME->gracz_A.wynik += gracz == PLAYER_A ? points : 0;
    GAME->gracz_B.wynik += gracz == PLAYER_B ? points : 0;
    paint_STATE(GAME->aside.window, GAME);
    GAME->ended = 1;
    char buffer[50];
    sprintf(buffer, "Player %s won! [ANY]", gracz == PLAYER_A ? "A" : "B");
    info(GAME->controls.window, buffer, ORANGE, gracz);
    pause();
    if (start_next(GAME, gracz)) {
        initGame(GAME);
        paint_GAMEVIEW(GAME);
        info(GAME->controls.window, "NEW GAME [ANY]", GREEN, gracz);
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
    W_GETNSTR_IN(1, 2, CTRLS_PADD + 1);
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

void overwrite_save(int curr) {
    FILE* file = fopen(SAVE_PATH, "r+");
    FILE* file2 = fopen(TEMP_PATH, "w+");
    char c;
    int lines = 1;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == '\n') lines++;
        if (lines == curr) break;
        fprintf(file2, "%c", c);
    }
    fclose(file);
    fclose(file2);
    remove(SAVE_PATH);
    rename(TEMP_PATH, SAVE_PATH);
}

int replay_interact(WINDOW* window, GAME_T* GAME, int curr, int limit,
                    int gracz) {
    char buffer[50];
    sprintf(buffer, TXT_REPLAY_1, curr, limit);
    info(window, buffer, GREEN, -1);
    switch (decide_replay(window, GAME)) {
        case 1:
            curr++;
            break;
        case 2:
            curr--;
            break;
        case 3:
            info(window, TXT_REPLAY_2, BLUE, -1);
            curr = get_number(window, GAME, -1);
            sprintf(buffer, TXT_REPLAY_3, curr);
            info(window, buffer, CYAN, -1);
            pause();
            clearLine(3);
            w_mvwprintw(2, getmaxx(window) - 1, "│");
            break;
        case 4:
            overwrite_save(curr);
            gameplay(GAME, gracz);
            return curr;

        default:
            break;
    }
    return curr;
}

void replay(GAME_T* GAME, FILE* file, int curr, int limit) {
    curr < 1 ? curr = limit : curr;
    curr > limit ? curr = 1 : curr;
    rewind(file);
    int gracz = load_save(GAME, file, 1, curr);
    paint_BOARD(GAME->plansza.window, GAME, BOARD_PADDING);
    paint_STATE(GAME->aside.window, GAME);
    WINDOW* win = GAME->controls.window;

    curr = replay_interact(win, GAME, curr, limit, gracz);

    wrefresh(win);
    replay(GAME, file, curr, limit);
}
int count_lines(FILE* file) {
    int lines = 1;
    char c;
    while (fscanf(file, "%c", &c) != EOF) {
        if (c == '\n') lines++;
    }
    lines--;
    lines < 1 ? lines = 1 : lines;
    rewind(file);
    return lines;
}
void run_replay(GAME_T* GAME) {
    paint_GAMEVIEW(GAME);
    FILE* file3 = fopen(SAVE_PATH, "r+");
    int limit = count_lines(file3);
    info(GAME->controls.window, TXT_REPLAY_0, GREEN, -1);
    pause();
    replay(GAME, file3, 1, limit);
    fclose(file3);
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
            // gracz = PLAYER_B;
            gracz = who_starts(GAME);
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
            run_replay(GAME);
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
    GAME->leftMoves = 0;
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