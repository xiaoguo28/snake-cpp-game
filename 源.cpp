#include <graphics.h>
#include <windows.h>
#include <time.h>

#define WIDTH  40
#define HEIGHT 30
#define SIZE   20

int snakeX[1000], snakeY[1000];
int foodX, foodY;
int len = 3;
int dir = 0;
bool gameOver = false;
bool isPaused = false;
int score = 0;
int speed = 150;
int bestScore = 0;

// 食物类型 0=普通 1=高分 2=减速
int foodType = 0;

void initFood() {
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
    int r = rand() % 100;
    if (r < 10)       foodType = 1;
    else if (r < 20)  foodType = 2;
    else              foodType = 0;
}

void initSnake() {
    len = 3;
    score = 0;
    speed = 150;
    snakeX[0] = 5; snakeY[0] = 5;
    snakeX[1] = 4; snakeY[1] = 5;
    snakeX[2] = 3; snakeY[2] = 5;
    dir = 0;
    gameOver = false;
    isPaused = false;
    initFood();
}

void drawBorder() {
    setlinestyle(PS_SOLID, 2);
    setlinecolor(RGB(40, 180, 255));
    rectangle(2, 2, WIDTH * SIZE - 2, HEIGHT * SIZE - 2);
}

void drawGrid() {
    setlinecolor(RGB(40, 40, 60));
    for (int i = 0; i <= WIDTH; i++)
        line(i * SIZE, 0, i * SIZE, HEIGHT * SIZE);
    for (int i = 0; i <= HEIGHT; i++)
        line(0, i * SIZE, WIDTH * SIZE, i * SIZE);
}

void drawScore() {
    setfillcolor(RGB(30, 30, 50));
    solidrectangle(5, 5, 220, 60);

    setbkmode(TRANSPARENT);
    settextcolor(RGB(240, 240, 255));
    settextstyle(22, 0, _T("楷体"));

    TCHAR str[50];
    wsprintf(str, _T("分数：%d"), score);
    outtextxy(12, 8, str);
    wsprintf(str, _T("最高分：%d"), bestScore);
    outtextxy(12, 33, str);
}

void drawGameOver() {
    settextcolor(BLACK);
    settextstyle(40, 0, _T("楷体"));
    outtextxy(302, 252, _T("游戏结束！"));
    outtextxy(275, 302, _T("按 R 重开 空格继续"));

    settextcolor(MAGENTA);
    outtextxy(300, 220, _T("游戏结束！"));
    outtextxy(230, 270, _T("按 R 重开 空格继续"));
}

void drawPause() {
    settextcolor(WHITE);
    settextstyle(50, 0, _T("楷体"));
    outtextxy(320, 220, _T("已暂停"));
}

void drawStartUI() {
    setbkcolor(RGB(20, 20, 35));
    cleardevice();
    settextcolor(RGB(30, 200, 255));
    settextstyle(60, 0, _T("楷体"));
    outtextxy(280, 100, _T("贪吃蛇"));
    settextstyle(28, 0, _T("楷体"));
    settextcolor(WHITE);
    outtextxy(240, 220, _T("1 简单   2 普通   3 困难"));
    outtextxy(270, 300, _T("按数字键开始游戏"));
}

void draw() {
    setbkcolor(RGB(20, 20, 35));
    cleardevice();

    drawGrid();
    drawBorder();

    int cx = foodX * SIZE + SIZE / 2;
    int cy = foodY * SIZE + SIZE / 2;

    if (foodType == 0) {
        setfillcolor(YELLOW);
        solidcircle(cx, cy, 4);
    }
    else if (foodType == 1) {
        setfillcolor(RED);
        solidcircle(cx, cy, 5);
    }
    else if (foodType == 2) {
        setfillcolor(GREEN);
        solidcircle(cx, cy, 5);
    }

    for (int i = 0; i < len; i++) {
        int x = snakeX[i] * SIZE;
        int y = snakeY[i] * SIZE;
        int g = 120 + i * 3;
        if (g > 220) g = 220;

        if (i == 0) {
            setfillcolor(RGB(30, 200, 255));
            solidroundrect(x + 2, y + 2, x + SIZE - 2, y + SIZE - 2, 5, 5);
            setfillcolor(WHITE);
            solidcircle(x + 7, y + 6, 2);
        }
        else {
            setfillcolor(RGB(20, g, 120));
            solidroundrect(x + 2, y + 2, x + SIZE - 2, y + SIZE - 2, 3, 3);
        }
    }

    drawScore();

    if (isPaused) drawPause();
    if (gameOver) drawGameOver();
}

void control() {
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (gameOver) {
            initSnake();
            Sleep(200);
        }
        else {
            isPaused = !isPaused;
            Sleep(200);
        }
    }

    if (isPaused) return;

    if (GetAsyncKeyState('W') & 0x8000)
        if (dir != 1) dir = 3;
    if (GetAsyncKeyState('S') & 0x8000)
        if (dir != 3) dir = 1;
    if (GetAsyncKeyState('A') & 0x8000)
        if (dir != 0) dir = 2;
    if (GetAsyncKeyState('D') & 0x8000)
        if (dir != 2) dir = 0;

    if (GetAsyncKeyState('R') & 0x8000) {
        initSnake();
        Sleep(200);
    }
}

void move() {
    if (gameOver || isPaused) return;

    for (int i = len - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    switch (dir) {
    case 0: snakeX[0]++; break;
    case 1: snakeY[0]++; break;
    case 2: snakeX[0]--; break;
    case 3: snakeY[0]--; break;
    }

    if (snakeX[0] < 0 || snakeX[0] >= WIDTH || snakeY[0] < 0 || snakeY[0] >= HEIGHT) {
        gameOver = true;
        if (score > bestScore) bestScore = score;

        // 死亡闪一下
        setbkcolor(RED);
        cleardevice();
        Sleep(30);
        return;
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        len++;
        if (foodType == 1)      score += 30;
        else if (foodType == 2) { score += 10; if (speed < 200) speed += 15; }
        else                    score += 10;

        initFood();
        if (speed > 40) speed -= 2;
    }
}

int main() {
    initgraph(WIDTH * SIZE, HEIGHT * SIZE);
    srand((unsigned)time(NULL));

    drawStartUI();
    while (true) {
        if (GetAsyncKeyState('1') & 0x8000) { speed = 180; break; }
        if (GetAsyncKeyState('2') & 0x8000) { speed = 120; break; }
        if (GetAsyncKeyState('3') & 0x8000) { speed = 70;  break; }
    }

    initSnake();

    while (true) {
        draw();
        control();
        move();
        Sleep(speed < 40 ? 40 : speed);
    }

    closegraph();
    return 0;
}
