#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#define KEY_SPACE 32
#define KEY_ESCAPE 27
#define KEY_C 99


// получение центра консольного окна для текста
void get_center(char *text, int *center)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    int size = strlen(text);
    center[0] = (row / 2);
    center[1] = (col - size) / 2;
}
// отрисовка бортика
void drawBorders(int posX, int posY, char *border)
{
    attron(COLOR_PAIR(1));
    mvprintw(posY - 1, posX, "%s", border);
    mvprintw(posY + 1, posX, "%s", border);
    mvprintw(posY, posX, "%s", "|");
    mvprintw(posY, posX + strlen(border) - 1, "%s", "|");
    attroff(COLOR_PAIR(1));
}
// смена элементов си-строки
void replacePairs(int cursorIndex, int emptyPos, char *text)
{
    char t = text[cursorIndex];
    text[cursorIndex] = text[emptyPos];
    text[emptyPos] = t;
    t = text[cursorIndex + 1];
    text[cursorIndex + 1] = text[emptyPos + 1];
    text[emptyPos + 1] = t;
}
// проверка строки на победное условие
bool checkTable(int count1, char *text, int size)
{
    int stCount = 0;
    for (int i = 0; text[i] != '2'; i++)
    {
        stCount += text[i] == '1';
    }
    return (stCount == count1);
}

// отрисовка ошибки
void renderError(int posX, int posY, char *text)
{
    attron(COLOR_PAIR(4));
    mvprintw(posY, posX, "%s", text);
    attroff(COLOR_PAIR(4));
}

// отрисовка количества ходов
void renderCountOfSteps(int countSpaces)
{
    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "Count of steps: %d; Click '0' to end the game", countSpaces);
    attroff(COLOR_PAIR(3));
}
// игровая сессия
void startgame()
{
    char oneOrTwo[2] = {'1', '2'}; // элементы для записи в строку
    int center[2]; // центр для строки
    int center2[2]; // центр для вспомогательных элементов дизайна
    int errorCenter[2]; // центр для отрисовки ощибки
    char *errorText = " ERROR! "; // текст для ошибки

    int size = (rand() % 4 + 4) * 2; // длина игровой строки
    char *text = (char *)malloc(size); // выделение памяти
    char *border = (char *)malloc(size + 2); // выделение памяти
    char *newText = (char *)malloc(size + 2); // выделение памяти

    // заполнение текста и бортиков
    for (int i = 0; i < size; i++)
    {
        text[i] = oneOrTwo[rand() % 2];
        border[i] = '-';
    }
    // добавление дополнительных частей бортика
    border[size] = '-';
    border[size + 1] = '-';
    // позиция крайней пустой клетки
    int emptyPos = rand() % (size - 1);
    text[emptyPos] = ' ';
    text[emptyPos + 1] = ' ';
    
    for (int i = 0; i < size; i++)
        newText[i + 1] = text[i];
    newText[size + 1] = '|';
    newText[0] = '|';

    // получение центров
    get_center(text, center);
    get_center(newText, center2);
    get_center(errorText, errorCenter);

    // будет использован для получения кодов клавиш
    int c = 0;
    
    // расчет начальных позиций
    int startX = center[1];
    int endX = startX + size - 1;
    int cPosX = rand() % (endX - startX + 1) + startX;
    int cPosY = center[0];
    int emptyIndex = emptyPos;
    int cursorIndex = cPosX - startX;
    int countSpaces = 0;

    // расчет кол-ва единиц
    int count1 = 0;
    for (int i = 0; i < size; i++)
    {
        count1 += (text[i] == '1');
    }

    // начальная открисовка
    drawBorders(center2[1], center2[0], border);
    mvprintw(center[0], center[1], "%s", text);
    renderCountOfSteps(countSpaces);
    move(cPosY, cPosX);
    refresh();

    bool endGame = false;

    while (!(endGame = checkTable(count1, text, size)) && (c = getch()) != 48)
    {
        clear();
        // расчет позиции курсора
        cPosX = cPosX + (c == KEY_RIGHT) - (c == KEY_LEFT);
        cPosX = (cPosX == endX + 1) ? startX : cPosX;
        cPosX = (cPosX == startX - 1) ? endX : cPosX;
        cursorIndex = cPosX - startX;
        // обработка пробела
        if (c == KEY_SPACE)
        {
            if (!(abs(cursorIndex - emptyIndex) <= 1 || cPosX == endX))
            {
                replacePairs(cursorIndex, emptyIndex, text);
                emptyIndex = cursorIndex;
                countSpaces++;
            }
            else
            {
                renderError(errorCenter[1], errorCenter[0] + 2, errorText);
            }
        }
        // отрисовка после изменений
        drawBorders(center2[1], center[0], border);
        mvprintw(center[0], center[1], "%s", text);
        renderCountOfSteps(countSpaces);
        move(cPosY, cPosX);
        refresh();
    }
    // конечный текст
    char *endText = " You have left the game without victory(. Press 'c' to restart or 'escape' to exit ";
    clear();
    // смена фона
    bkgd(COLOR_PAIR(4));
    if (endGame)
    {
        endText = " Congratulations! You have won this game!. Press 'c' to restart or 'escape' to exit ";
        bkgd(COLOR_PAIR(2));
    }
    get_center(endText, center);
    // отрисовка конечного текста
    mvprintw(center[0], center[1], "%s", endText);
    refresh();
}
int main()
{
    // инициализиция 
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    start_color();
    // цвета
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_RED);
    bkgd(COLOR_PAIR(3));
    // приветствие
    char* welcomeText = "WELCOME TO THE GAME. Click 'c' to start or 'escape' to exit!";
    int welcomeCenter[2];
    get_center(welcomeText, welcomeCenter);
    mvprintw(welcomeCenter[0], welcomeCenter[1], "%s", welcomeText);
    int btn = 0;
    // главный цикл
    while ((btn = getch()) != KEY_ESCAPE) {
        if (btn == KEY_C) {
            clear();
            bkgd(A_NORMAL);
            startgame();
        }
    }
    endwin();
    return 0;
}
