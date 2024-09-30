#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void get_center(char* text, int *center) {
    int row, col;
    getmaxyx(stdscr, row, col);
    int size = strlen(text);
    center[0] = (row / 2);
    center[1] = (col - size) / 2;
}

int main() {
    srand(time(NULL));
    initscr();              
    cbreak();               
    noecho();               
    keypad(stdscr, TRUE); 
    curs_set(1);

    char oneOrTwo[2] = {'1', '2'};
    int center[2];
    int center2[2];


    int size = (rand() % 4 + 4)* 2;
    char* text = (char*)malloc(size);
    char* border = (char*)malloc(size+2);
    char* newText = (char*)malloc(size + 2);
    for (int i = 0; i< size; i++) {
        text[i] = oneOrTwo[rand()%2];
        border[i] = '-';
    }
    border[size] = '-';
    border[size+1] = '-';
    int emptyPos = rand() % (size-1);
    text[emptyPos] = ' ';
    text[emptyPos+1] = ' ';
    for (int i = 0; i<size; i++) newText[i+1] = text[i];
    newText[size + 1] = '|';
    newText[0]= '|';
    get_center(text, center);
    get_center(newText, center2);


    int c = 0;
    int startX = center[1];
    int cPosX = center[1];
    int cPosY = center[0];
    int endX = startX + size - 1;
    mvprintw(center2[0]-1, center2[1], "%s", border);
    mvprintw(center2[0]+1, center2[1], "%s", border);
    mvprintw(center2[0], center2[1], "%s", newText);
    move(cPosY, cPosX);
    refresh();

    while ((c = getch()) != 48) {
        cPosX = cPosX + (c == KEY_RIGHT) - (c == KEY_LEFT);
        cPosX = (cPosX == endX + 1) ? startX : cPosX;
        cPosX = (cPosX == startX -1) ? endX : cPosX;
        clear();
        mvprintw(center[0]-1, center2[1], "%s", border);
        mvprintw(center[0]+1, center2[1], "%s", border);
        mvprintw(center2[0], center2[1], "%s", newText);
        move(cPosY, cPosX);
        refresh();
    }

    endwin();  // Завершаем работу с ncurses
    return 0;
}
