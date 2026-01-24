#include <iostream>
#include <fstream>
#include "help.h"
using namespace std;

const int rows = 6;
const int cols = 10;
const int brickwidth = 60;
const int brickheight = 20;
const int brickpadding = 10;
const int screenwidth = 1024;
const int screenheight = 600;

const int totalbrickwidth = cols * brickwidth + (cols - 1) * brickpadding;
const int totalbrickheight = rows * brickheight + (rows - 1) * brickpadding;
const int brickstartx = (screenwidth - totalbrickwidth) / 2 - 30;
const int brickstarty = 120;

const int margin = 30;
const int left_limit = brickstartx - margin;
const int right_limit = brickstartx + totalbrickwidth + margin;
const int top_limit = brickstarty - margin;
const int bottom_limit = 460;

const int paddlewidth = 80;
const int paddleheight = 10;

bool bricks[rows][cols];

void saveGame(int ball_x, int ball_y, int ball_dx, int ball_dy, int paddle_x, int score, int lives, bool bricks[rows][cols])
{
    ofstream out("savegame.txt");

    out << ball_x << " " << ball_y << " " << ball_dx << " " << ball_dy << " ";
    out << paddle_x << " " << score << " " << lives << "\n";

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            out << bricks[i][j] << " ";

    out.close();
}

bool loadGame(int& ball_x, int& ball_y, int& ball_dx, int& ball_dy, int& paddle_x, int& score, int& lives, bool bricks[rows][cols])
{
    ifstream in("savegame.txt");

    if (!in)
        return false;

    in >> ball_x >> ball_y >> ball_dx >> ball_dy;
    in >> paddle_x >> score >> lives;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)

            in >> bricks[i][j];
    in.close();

    return true;
}

void updateHighScores(int score)
{
    int scores[6], count = 0;

    ifstream in("highscores.txt");

    while (in >> scores[count] && count < 5)
        count++;

    in.close();

    scores[count++] = score;

    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (scores[i] < scores[j])
                swap(scores[i], scores[j]);

    ofstream out("highscores.txt");

    for (int i = 0; i < 5 && i < count; i++)
        out << scores[i] << "\n";

    out.close();
}

void showHighScores()
{
    ifstream in("highscores.txt");

    int score, y = 200;

    myRect(0, 0, 1024, 600, 0, 0, 0);
    drawText(22, 300, 130, 255, 255, 0, "TOP 5 HIGH SCORES");

    while (in >> score)
    {
        drawText(18, 380, y, 255, 255, 255, "score: ", true, score);
        y += 40;
    }

    in.close();
    Sleep(3000);
}

void drawpaddle(int x, int y) {
    myRect(x - 2, y - 2, x + paddlewidth, y + paddleheight, 255, 255, 255, 255, 255, 0);
}
void removepaddle(int x, int y)
{
    myRect(x - 2, y - 2, x + paddlewidth, y + paddleheight, 0, 0, 0);
}
void drawball(int x, int y)
{
    myEllipse(x, y, x + 10, y + 10, 255, 0, 0, 255, 0, 0);
}
void removeball(int x, int y)
{
    myEllipse(x, y, x + 10, y + 10, 0, 0, 0);
}
void drawbrick(int row, int col)
{
    int x = brickstartx + col * (brickwidth + brickpadding);
    int y = brickstarty + row * (brickheight + brickpadding);

    myRect(x, y, x + brickwidth, y + brickheight, 255, 255, 255, 0, 255, 255);
}
void removebrick(int row, int col)
{
    int x = brickstartx + col * (brickwidth + brickpadding);
    int y = brickstarty + row * (brickheight + brickpadding);

    myRect(x, y, x + brickwidth, y + brickheight, 0, 0, 0);
}
void drawallbricks(int brickRows)
{
    for (int i = 0; i < brickRows; i++)
        for (int j = 0; j < cols; j++)
            if (bricks[i][j])
                drawbrick(i, j);
}

int showMainMenu()
{
    int selection = 1;
    int whichKey = 0;
    bool selected = false;

    while (!selected)
    {
        myRect(0, 0, 1024, 600, 0, 0, 0);
        drawText(22, 370, 100, 0, 255, 0, "BREAKOUT GAME");

        char items[4][30] = {
            "1. START NEW GAME",
            "2. LOAD GAME",
            "3. VIEW HIGH SCORES",
            "4. QUIT"
        };

        for (int i = 0; i < 4; i++)
        {
            int y = 180 + i * 50;
            if (i + 1 == selection)
                drawText(18, 360, y, 255, 255, 0, items[i]);
            else
                drawText(18, 360, y, 255, 255, 255, items[i]);
        }

        Sleep(1000);

        if (isKeyPressed(whichKey))
        {
            if (whichKey == 2 && selection > 1)
                selection--;
            else if (whichKey == 4 && selection < 4)
                selection++;
            else if (whichKey == 5)
                selected = true;
        }
    }
    return selection;
}

int chooseDifficulty()
{
    int selection = 1;
    int whichKey = 0;
    bool selected = false;

    while (!selected)
    {
        myRect(0, 0, 1024, 600, 0, 0, 0);
        drawText(22, 330, 100, 255, 255, 0, "CHOOSE DIFFICULTY");

        char levels[2][20] = { "1. NORMAL", "2. HARD" };

        for (int i = 0; i < 2; i++)
        {
            int y = 200 + i * 60;

            if (i + 1 == selection)
                drawText(18, 400, y, 255, 255, 0, levels[i]);
            else
                drawText(18, 400, y, 255, 255, 255, levels[i]);
        }

        Sleep(1000);

        if (isKeyPressed(whichKey))
        {
            if (whichKey == 2 && selection > 1)
                selection--;
            else if (whichKey == 4 && selection < 2)
                selection++;
            else if (whichKey == 5)
                selected = true;
        }
    }
    return selection;
}

int main()
{
    int choice = showMainMenu();
    int paddle_x, paddle_y = bottom_limit - 20;
    int ball_x, ball_y, ball_dx, ball_dy;
    int score = 0, lives = 3;
    int whichkey = 0;
    char direction = ' ';
    const int step_size = 5;
    int difficulty = 1;
    int brickRows = 4;

    if (choice == 4)
        return 0;

    if (choice == 3)
    {
        showHighScores();
        return 0;
    }

    if (choice == 2)
    {
        if (!loadGame(ball_x, ball_y, ball_dx, ball_dy, paddle_x, score, lives, bricks))
        {
            drawText(20, 300, 500, 255, 0, 0, "No saved game found!");
            Sleep(2000);
            return 0;
        }
    }
    else
    {
        difficulty = chooseDifficulty();

        brickRows = (difficulty == 1) ? 4 : 6;

        paddle_x = (left_limit + right_limit - paddlewidth) / 2;

        ball_x = paddle_x + 35;
        ball_y = paddle_y - 12;

        ball_dx = (difficulty == 1 ? 3 : 4);
        ball_dy = (difficulty == 1 ? -3 : -4);

        for (int i = 0; i < brickRows; i++)
            for (int j = 0; j < cols; j++)
                bricks[i][j] = (difficulty == 1) ? true : ((i + j) % 2 == 0);
    }

    myRect(left_limit - 5, top_limit - 4, right_limit + 5, bottom_limit + 4, 255, 0, 0);

    int text_x = right_limit - 150;
    int score_y = top_limit - 70;
    int lives_y = top_limit - 35;

    drawText(16, text_x, score_y, 255, 255, 255, "score: ", true, score);
    drawText(16, text_x, lives_y, 255, 255, 255, "lives: ", true, lives);

    drawallbricks(brickRows);
    drawpaddle(paddle_x, paddle_y);
    drawball(ball_x, ball_y);

    while (whichkey != 7)
    {
        if (isKeyPressed(whichkey))
        {
            switch (whichkey)
            {
            case 1: direction = 'l';
                break;
            case 3: direction = 'r';
                break;
            default: direction = ' ';
                break;
            }
        }

        if (GetAsyncKeyState('S') & 0x8000)
        {
            saveGame(ball_x, ball_y, ball_dx, ball_dy, paddle_x, score, lives, bricks);
            drawText(16, 400, 500, 0, 255, 0, "Game Saved!");
        }

        Sleep(30);
        removepaddle(paddle_x, paddle_y);

        if (direction == 'l' && paddle_x > left_limit)
            paddle_x -= step_size;

        if (direction == 'r' && paddle_x + paddlewidth < right_limit)
            paddle_x += step_size;

        drawpaddle(paddle_x, paddle_y);

        removeball(ball_x, ball_y);

        ball_x += ball_dx;
        ball_y += ball_dy;

        if (ball_x <= left_limit || ball_x + 10 >= right_limit)
            ball_dx = -ball_dx;

        if (ball_y <= top_limit)
            ball_dy = -ball_dy;

        if (ball_y + 10 >= paddle_y && ball_y + 10 <= paddle_y + paddleheight &&
            ball_x + 5 >= paddle_x && ball_x + 5 <= paddle_x + paddlewidth)
        {
            ball_dy = -ball_dy;
            ball_y = paddle_y - 11;
        }

        bool brickHit = false;

        for (int i = 0; i < brickRows && !brickHit; i++)
        {
            for (int j = 0; j < cols && !brickHit; j++)
            {
                if (bricks[i][j])
                {
                    int bx = brickstartx + j * (brickwidth + brickpadding);
                    int by = brickstarty + i * (brickheight + brickpadding);

                    if (ball_x + 5 >= bx && ball_x + 5 <= bx + brickwidth &&
                        ball_y + 5 >= by && ball_y + 5 <= by + brickheight)
                    {
                        bricks[i][j] = false;

                        removebrick(i, j);

                        ball_dy = -ball_dy;
                        score += (difficulty == 2 ? 5 : 7);

                        myRect(text_x, score_y, text_x + 150, score_y + 30, 0, 0, 0);
                        drawText(16, text_x, score_y, 255, 255, 255, "score: ", true, score);

                        brickHit = true;
                    }
                }
            }
        }

        if (ball_y + 10 >= bottom_limit)
        {
            lives--;

            drawText(16, text_x, lives_y, 255, 255, 255, "lives: ", true, lives);

            if (lives == 0)
            {
                drawText(22, 360, 360, 255, 0, 0, "game over!");
                updateHighScores(score);
                break;
            }
            else
            {
                ball_x = paddle_x + 35;
                ball_y = paddle_y - 12;
                ball_dx = (difficulty == 1 ? 3 : 4);
                ball_dy = (difficulty == 1 ? -3 : -4);
                Sleep(1000);
            }
        }

        drawball(ball_x, ball_y);

        bool allbroken = true;

        for (int i = 0; i < brickRows && allbroken; i++)
            for (int j = 0; j < cols && allbroken; j++)
                if (bricks[i][j])
                    allbroken = false;

        if (allbroken)
        {
            drawText(22, 360, 360, 0, 255, 0, "you win!");
            updateHighScores(score);
            break;
        }
    }

    return 0;
}