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
// void replaceThem(char& ch1, char& ch2) {
//     char n1 = ch1;
//     ch1 = ch2;
//     ch2 = n1;
// }

// void replace_mas(char* text, int emptyPos, int cursorPos) {
//     replaceThem(text[emptyPos], text[cursorPos]);
//     replaceThem(text[emptyPos+1], text[cursorPos+1]);
// }

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    //srand(time(NULL));

    char oneOrTwo[2] = {'1', '2'};
    int center[2];
    const char* text = "PRIVET QWE";
    get_center(text, center);
    int c = 1;
    int cPosX = center[0];
    int cPosY = center[1];
    while ((c = getch())!= 48) {
        move(cPosY, cPosX);
        mvprintw(center[0], center[1], "%s", text);
        refresh();
        c = getch();
        if (c == KEY_RIGHT) {
            cPosX--;
        }
        else if (c == KEY_LEFT) {
            cPosX++;
        }
    }
    refresh();

    getch();
    endwin();
    return 0;
}
