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