
// Stałe
#define TRUE 1
#define FALSE 0
#define MAX_HALL_OF_FAME 10
#define MAX_NAME 20

// Znaki
// #define BLOCK "\u25A7"
#define BLOCK "\u25A0"
#define PIONEK "\u25CF"
#define POLE_UP "\u25B3"
#define POLE_DOWN "\u25BD"
#define LINE "\u2507"

// Kolory
#define COLOR_DEFAULT 0
#define BLUE_YELLOW 1
#define RED 2
#define YELLOW 3

// Funkcje
#define CLR(x) COLOR_PAIR(x)

// Struktury
struct POLE_T {
    int liczba;  // 0 - brak, 1-15 - ilość pionków
    int kolor;   // 0 - brak, 1 - gracz A, 2 - gracz B
};
struct BAR_T {
    int gracz_A;  // 0 - brak, 1-15 - ilość pionków
    int gracz_B;  // 0 - brak, 1-15 - ilość pionków
};
struct DWOR_T {
    int gracz_A;  // 0 - brak, 1-15 - ilość pionków
    int gracz_B;  // 0 - brak, 1-15 - ilość pionków
};
struct PLANSZA_T {
    struct POLE_T pole[24];
    struct BAR_T bar;
    struct DWOR_T dwor;
    // struct KOSTKA_T kostka;
};
struct GRACZ_T {
    char nazwa[MAX_NAME];
    int wynik;
};
struct GAME_T {
    struct PLANSZA_T plansza;
    struct GRACZ_T gracz_A;
    struct GRACZ_T gracz_B;
};
struct HALL_OF_FAME_T {
    struct GRACZ_T gracz[MAX_HALL_OF_FAME];
};
