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

#define CLR_PLAYER_A MAGENTA
#define CLR_PLAYER_B CYAN

#define STARTED 0
#define PLAYING 1
#define ENDED 2

#define PIONKI_HEIGHT 5
#define BOARD_PADDING 2
#define BOARD_WIDTH 47
#define PADDED_BOARD_WIDTH (BOARD_WIDTH + (BOARD_PADDING * 3))
#define PADDED_BOARD_HEIGHT (BOARD_HEIGHT + (BOARD_PADDING * 2))

#define CONTROLS_PADD 15 + 2  // strlen("Ruch-Gracza X: ") + x
#define MENU_PADD 18 + 3      // strlen("Choose from list: ") + x

#define BOARD_HEIGHT 13

// FILE PATHS
#define FAME_PATH "ARCHIVE/fame.txt"
#define SAVE_PATH "ARCHIVE/save.txt"

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

// Struktury
struct POLE_T {
    int liczba;  // 0 - brak, 1-15 - ilość pionków
    int kolor;   // 0 - brak, 1 - gracz A, 2 - gracz B
    int number;  // number of field
};
struct DWOR_T {
    int gracz_A;  // 0 - brak, 1-15 - ilość pionków
    int gracz_B;  // 0 - brak, 1-15 - ilość pionków
};
struct PLANSZA_T {
    WINDOW* window;
    struct POLE_T pole[POLE_COUNT + 3];  // 0 - bar A, 25 - bar B, 26 - trash
    struct DWOR_T dwor;
};
struct GRACZ_T {
    char nazwa[MAX_NAME];
    int wynik;
};
struct FAME_T {
    struct GRACZ_T gracz[MAX_FAME];
};
struct UI_T {
    WINDOW* window;
};

struct GAME_T {
    struct PLANSZA_T plansza;
    struct GRACZ_T gracz_A;
    struct GRACZ_T gracz_B;
    struct FAME_T fame;
    struct UI_T aside;
    struct UI_T controls;
    struct UI_T ui_2;
    struct UI_T menu;
    int dice[4];
    char komunikat[MAX_LONG_STR];
    int rand_seed;
    int dublet;
    int pozostałe_ruchy;
    char ruchy[MAX_LONG_STR];
    int home_news;
    int turn;
    int ended;
};

struct MOVE_T {
    int kostka;
    int pionek;
};