
// Stałe
#define TRUE 1
#define FALSE 0
#define MAX_HALL_OF_FAME 10
#define MAX_NAME 20
#define MAX_BAR 6
#define POLE_COUNT 24
#define MAX_LONG_STR 200
#define MAX_SHORT_STR 20

#define PLAYER_A 1
#define PLAYER_B 2

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

#define controls_padd 15 + 2  // strlen("Ruch-Gracza X: ")
#define menu_padd 18 + 3      // strlen("Choose from list: ")
#define controls_promt "R(oll) | M(ove) | S(kip) | E(xit)"

#define BOARD_HEIGHT 13

// Znaki
// #define BLOCK "\u25A7"
#define BLOCK "\u25A0"
#define PIONEK "\u25CF"
#define POLE_UP "\u25B3"
#define POLE_DOWN "\u25BD"
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

#define COLOR_BROWN 80
#define COLOR_DARK_BLUE 81

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
    wrefresh(window);                         \
    w_mvwprintw(y, 0, "│");

#define pause() \
    ;           \
    noecho();   \
    getch();    \
    echo();

// Struktury
struct POLE_T {
    int liczba;  // 0 - brak, 1-15 - ilość pionków
    int kolor;   // 0 - brak, 1 - gracz A, 2 - gracz B
    int xyPos;   // default specifing position on window
    int number;  // number of pole
};
struct DWOR_T {
    int gracz_A;  // 0 - brak, 1-15 - ilość pionków
    int gracz_B;  // 0 - brak, 1-15 - ilość pionków
};
struct PLANSZA_T {
    WINDOW* window;
    struct POLE_T pole[POLE_COUNT + 2];
    struct DWOR_T dwor;
};
struct GRACZ_T {
    char nazwa[MAX_NAME];
    int wynik;
};
struct HALL_OF_FAME_T {
    struct GRACZ_T gracz[MAX_HALL_OF_FAME];
};
struct UI_T {
    WINDOW* window;
};

struct GAME_T {
    struct PLANSZA_T plansza;
    struct GRACZ_T gracz_A;
    struct GRACZ_T gracz_B;
    struct HALL_OF_FAME_T hall_of_fame;
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
};

struct MOVE_T {
    int kostka;
    int pionek;
};