#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>

void get_center(const char* text, int *center) {
    int row, col;
    getmaxyx(stdscr, row, col);
    int textLength = strlen(text);
    center[0] = (row / 2);
    center[1] = (col - textLength) / 2;
}

int main() {
    initscr();              
    cbreak();               
    noecho();               
    keypad(stdscr, TRUE); 

    char oneOrTwo[2] = {'1', '2'};
    int center[2];
    const char* text = "PRIVET QWE";
    get_center(text, center);

    int c = 0;
    int cPosY = center[0];
    int cPosX = center[1];
    mvprintw(cPosY, cPosX, "%s", text);
    move(cPosY, cPosX);
    refresh();

    while ((c = getch()) != 48) {
        switch (c) {
            case KEY_RIGHT:
                cPosX++;
                break;
            case KEY_LEFT:
                cPosX--;
                break;
        }
        clear();
        mvprintw(center[0], center[1], "%s", text);
        move(cPosY, cPosX);
        refresh();
    }

    endwin();  // Завершаем работу с ncurses
    return 0;
}
