// fun.c
void colorInit();
char initialInit();
void initWindows(struct GAME_T* GAME);

// paint.c
void paint_POLE_TOP(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole);
void paint_POLE_BOT(int x, int pad, int kolor, WINDOW* window,
                    struct POLE_T pole);
void paint_BAR_A(int x, int y, WINDOW* window, struct GAME_T* GAME);
void paint_BAR_B(int x, int y, WINDOW* window, struct GAME_T* GAME);
void paint_BAR(int x, int y, WINDOW* window, struct GAME_T* GAME);
void paint_DWOR(int x, int y, WINDOW* window, struct DWOR_T dwor);
void paint_BOARD(WINDOW* window, struct GAME_T* GAME, int pad);