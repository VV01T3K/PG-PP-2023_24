// fun.c
void colorInit();
char init();

// paint.c
void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole);
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole);
void paint_BAR_A(int x, int y, WINDOW* window, struct BAR_T bar);
void paint_BAR_B(int x, int y, WINDOW* window, struct BAR_T bar);
void paint_BAR(int x, int y, WINDOW* window, struct BAR_T bar);