#include <locale.h>
#include <ncurses.h>
#include <stdio.h>

#include "fun.h"

int main() {
    testowanko();
    printf("Hello World2\n");

    setlocale(LC_ALL, "");     // Ustawienie lokalizacji
    initscr();                 // Start
    if (has_colors() == TRUE)  // 1
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_YELLOW);
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
        printw("unicode: \u03A9 \n");
        printw("ascii: %c \n", 0x03);
        printw("hex: %x \n", 0x03);
        printw("dec: %d \n", 0x03);
        printw("oct: %o \n", 0x03);
        printw("Kolorowy napis. ;P");
        printw("\n");
        attroff(COLOR_PAIR(1));  // Wyłączenie koloru tekstu
    } else {
        printw("Twoja Konsolka nie obsluguje kolorow. :/ ");
    }
    getch();
    endwin();  // koniec
    return 0;
}