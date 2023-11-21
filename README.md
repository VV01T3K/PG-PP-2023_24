# PP_CPP_AlpineLinux

https://www.asciitable.com/


apk search ncurses
apk search ncurses | grep 'dev'
apk search ncurses | grep -- '-dev'
apk info -qL ncurses-dev

gcc -o .game index.c fun.c $(ncursesw6-config --cflags --libs) && ./.game
