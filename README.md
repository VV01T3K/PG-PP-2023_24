# PP_CPP_AlpineLinux

https://www.asciitable.com/

https://www.w3.org/TR/xml-entity-names/025.html

apk search ncurses
apk search ncurses | grep 'dev' 
apk search ncurses | grep -- '-dev'
apk info -qL ncurses-dev

gcc -o .game index.c fun.c paint.c $(ncursesw6-config --cflags --libs) && ./.game

gcc -o .game index.c fun.c paint.c $(ncursesw6-config --cflags --libs) && ./.game &&  rm .game && clear


sudo update-locale LANG=en_US.UTF-8 LANGUAGE=en.UTF-8


1. Compile your program with debugging symbols: gcc -g your_program.c -o your_program
2. Run it in the debugger: gdb your_program
3. In the gdb console, run your program by typing run. The program will stop if it hits a segfault.
4. To find out where it crashed, type backtrace. This will give you the function call stack, showing you exactly where the crash happened.