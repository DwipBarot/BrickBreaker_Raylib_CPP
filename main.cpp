#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>
using namespace std;

const int screen_width = 900;
const int screen_height = 950;

Sound hitbrick;
Sound winsound;
Sound gameoversound;

bool IsWin = false;

int score = 0;
int lives = 5;

enum GameState
{
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
};

GameState gameState = MENU;

// Brick details

const int BRICK_ROWS = 6;
const int BRICK_COLS = 10;
const float BRICK_WIDTH = 70;
const float BRICK_HEIGHT = 25;
const float BRICK_PADDING = 10;
const float BRICK_OFFSET_TOP = 60;
const float BRICK_OFFSET_LEFT = 50;

// Brick class
class Brick
{
public:
    Rectangle rect;
    bool active;
    int health;

    Brick(float x, float y, float w, float h, int hp)
    {
        rect = {x, y, w, h};
        health = hp;
        active = true;
    }

    Color getColor() const // colour representing the health of brick
    {
        if (health == 3)
            return RED;
        if (health == 2)
            return ORANGE;
        return GREEN;
    }

    void draw()
    {
        if (active)
        {
            DrawRectangleRec(rect, getColor());
            DrawRectangleLinesEx(rect, 1, GRAY); // nice outline
        }
    }
};
class Paddle
{
public:
    float x, y;
    float speed_x;
    int width, height;

    void draw()
    {
        DrawRectangle(x, y, width, height, BLUE);
    }

    void limitmovement()
    {
        if (x <= 0)
        {
            x = 0;
        }
        if (x >= GetScreenWidth() - width)
        {
            x = GetScreenWidth() - width;
        }
    }

    void update()
    {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            x += speed_x;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            x -= speed_x;
        }
        limitmovement();
    }
};

class Ball
{
public:
    float x, y;
    float speed_x, speed_y;
    float radius;
    bool islaunch = false;
    float launchAngle = 90 * DEG2RAD; // straight up
    float maxAngle = 60 * DEG2RAD;    // left/right limit
    float aimSpeed = 3 * DEG2RAD;     // rotation speed

    void draw()
    {
        DrawCircle(x, y, radius, YELLOW);
    }

    void resetball(Paddle &paddle)
    {
        paddle.x = screen_width / 2 - paddle.width / 2;
        paddle.y = screen_height - paddle.height - 20;

        x = paddle.x + paddle.width / 2;
        y = paddle.y - radius;
    }

    void launchBall()
    {
        if (!islaunch)
        {
            float speed = sqrt(speed_x * speed_x + speed_y * speed_y);

            speed_x = speed * cos(launchAngle);
            speed_y = -speed * sin(launchAngle);

            islaunch = true;
        }
    }

    void update(Paddle &paddle, vector<Brick> &bricks)
    {
        if (islaunch == true)
        {
            x += speed_x;
            y += speed_y;

            if ((x - radius <= 0) || (x + radius >= GetScreenWidth()))
            {
                speed_x = -speed_x;
            }
            if ((y - radius <= 0))
            {
                speed_y = -speed_y;
            }
            if ((y + radius >= GetScreenHeight()))
            {
                lives--;
                islaunch = false; // helps to reset the ball
                resetball(paddle);
                if (lives == 0)
                {
                    PlaySound(gameoversound);
                    gameState = GAMEOVER;
                }
            }

            Rectangle paddleRect = {
                paddle.x,
                paddle.y,
                (float)paddle.width,
                (float)paddle.height};

            if (CheckCollisionCircleRec({x, y}, radius, paddleRect))
            {
                // Always bounce up
                y = paddle.y - radius; // helps in removing jitter of the ball if ball.y > paddle.y (in a frame speed y is increased by 7) // prevents the ball from going inside the paddle
                speed_y = -abs(speed_y);

                float hitX = x - paddle.x; // collision point relative to paddle
                /*Explaination of hitX

                --paddle.x = 300 and paddle.width = 140
                --x = 305 thus hitX = 305 - 300 = 5

                and therfore hitx give the position of collision on paddle

                **/

                float relativeHit = (hitX - paddle.width / 2) / (paddle.width / 2); // this gives the range of -1 to 1 for left(-1) to right(1) edge 0 for center
                float maxAngle = 45 * DEG2RAD;                                      // accepts radians for angle
                float bounceAngle = relativeHit * maxAngle;

                float speed = sqrt((speed_x) * (speed_x) + (speed_y) * (speed_y));

                speed_x = speed * sin(bounceAngle);
                speed_y = -speed * cos(bounceAngle);
            }
        }
        else
        {
            x = paddle.x + paddle.width / 2;
            y = paddle.y - radius;

            if (IsKeyPressed(KEY_Z))
                launchAngle += aimSpeed;

            if (IsKeyPressed(KEY_X))
                launchAngle -= aimSpeed;

            if (launchAngle > 90 * DEG2RAD + maxAngle)
                launchAngle = 90 * DEG2RAD + maxAngle;

            if (launchAngle < 90 * DEG2RAD - maxAngle)
                launchAngle = 90 * DEG2RAD - maxAngle;
        }

        for (auto &brick : bricks)
        {
            if (!brick.active)
                continue;

            if (CheckCollisionCircleRec({x, y}, radius, brick.rect))
            {

                PlaySound(hitbrick);
                // Find ball center
                float ballCenterX = x;
                float ballCenterY = y;

                // Find brick center
                float brickCenterX = brick.rect.x + brick.rect.width / 2;
                float brickCenterY = brick.rect.y + brick.rect.height / 2;

                // Distance between centers
                float diffX = ballCenterX - brickCenterX;
                float diffY = ballCenterY - brickCenterY;

                // Overlap distances
                float overlapX =
                    (brick.rect.width / 2 + radius) - fabs(diffX);
                float overlapY =
                    (brick.rect.height / 2 + radius) - fabs(diffY);

                // Resolve collision on smaller overlap
                if (overlapX < overlapY)
                {
                    speed_x = -speed_x;
                }
                else
                {
                    speed_y = -speed_y;
                }

                // Damage brick
                brick.health--;

                if (brick.health <= 0)
                {
                    brick.active = false;
                    score += 50;
                }
                else
                {
                    score += 10;
                }

                // only one brick per frame
                break;
            }
        }
    }

    void drawAimLine()
    {
        if (islaunch)
            return;

        float dotSpacing = 15.0f;
        int dotCount = 10;

        for (int i = 1; i <= dotCount; i++)
        {
            float dx = cos(launchAngle) * dotSpacing * i;
            float dy = -sin(launchAngle) * dotSpacing * i;

            DrawCircle(x + dx, y + dy, 3, Fade(WHITE, 0.7f));
        }
    }
};

bool AreallBricksDestroyed(const vector<Brick> &bricks)
{
    for (const auto &brick : bricks)
    {
        if (brick.active)
            return false;
    }
    return true;
}

void UpdateMenu()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        score = 0;
        gameState = PLAYING;
    }
}
void DrawMenu()
{
    DrawText("BRICK BREAKER", 250, 450, 50, WHITE);
    DrawText("Press ENTER to Start", 320, 520, 24, GRAY);
    DrawText("Made By marvelboyop & Dwip", 310, 560, 20, WHITE);
}

void UpdateGame(Ball &ball, Paddle &paddle, vector<Brick> &bricks)
{
    if (IsKeyPressed(KEY_P))
    {
        gameState = PAUSED;
        return;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        ball.launchBall();
    }

    if (AreallBricksDestroyed(bricks) && gameState == PLAYING)
    {
        PlaySound(winsound);
        IsWin = true;
        gameState = GAMEOVER;
    }

    paddle.update();
    ball.update(paddle, bricks);
}
void DrawGame(Ball &ball, Paddle &paddle, vector<Brick> &bricks)
{
    paddle.draw();
    ball.draw();
    ball.drawAimLine();

    for (auto &brick : bricks)
        brick.draw();

    DrawText(TextFormat("Score: %d", score), 20, 20, 28, WHITE);
    DrawText(TextFormat("Lives: %d", lives), GetScreenWidth() - 120, 20, 28, WHITE);
}

void UpdatePause()
{
    if (IsKeyPressed(KEY_P))
    {
        gameState = PLAYING;
    }
}
void DrawPause()
{
    DrawText("PAUSED", 360, 450, 40, YELLOW);
    DrawText("Press P to Resume", 300, 500, 30, GRAY);
}

void DrawGameOver()
{
    if (IsWin)
    {
        DrawText("YOU WIN!!!", 310, 400, 60, BLUE);
    }
    else
    {
        DrawText("GAME OVER", 260, 400, 60, RED);
    }

    DrawText(TextFormat("Final Score: %d", score), 320, 480, 30, WHITE);
    DrawText("PRESS ENTER TO RESTART", 270, 540, 24, GRAY);
}

void resetgame(Ball &ball, Paddle &paddle, vector<Brick> &bricks)
{
    lives = 5;
    score = 0;

    paddle.x = screen_width / 2 - paddle.width / 2;
    paddle.y = screen_height - paddle.height - 20;

    ball.islaunch = false;
    ball.launchAngle = 90 * DEG2RAD;
    ball.speed_x = 7;
    ball.speed_y = -7;

    ball.x = paddle.x + paddle.width / 2;
    ball.y = screen_height - paddle.height - 20 - ball.radius;

    bricks.clear();

    for (int row = 0; row < BRICK_ROWS; row++)
    {
        int hp;
        if (row == 0 || row == 1)
        {
            hp = 3;
        }
        else if (row == 2 || row == 3)
        {
            hp = 2;
        }
        else if (row == 4 || row == 5)
        {
            hp = 1;
        }
        for (int col = 0; col < BRICK_COLS; col++)
        {
            float x = BRICK_OFFSET_LEFT + col * (BRICK_WIDTH + BRICK_PADDING);
            float y = BRICK_OFFSET_TOP + row * (BRICK_HEIGHT + BRICK_PADDING);

            bricks.emplace_back(x, y, BRICK_WIDTH, BRICK_HEIGHT, hp);
        }
    }
}

void UpdateGameOver(Ball &ball, Paddle &paddle, vector<Brick> &bricks)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        score = 0;
        IsWin = false;
        resetgame(ball, paddle, bricks);
        gameState = MENU;
    }
}

int main()
{
    InitWindow(screen_width, screen_height, "Brick Breaker made by marvelboyop & Dwip");
    SetTargetFPS(60);

    InitAudioDevice(); // for audio
    // Load sounds AFTER audio init

    hitbrick = LoadSound("Assets/Sounds/Hitbrick.wav");
    gameoversound = LoadSound("Assets/Sounds/gameover.wav");
    winsound = LoadSound("Assets/Sounds/win.wav");

    // Paddle details
    Paddle paddle;
    paddle.width = 140;
    paddle.height = 20;
    paddle.x = screen_width / 2 - paddle.width / 2;
    paddle.y = screen_height - paddle.height - 20;
    paddle.speed_x = 6;

    // Ball details
    Ball ball;
    ball.x = paddle.x + paddle.width / 2;
    ball.radius = 15;
    ball.y = screen_height - paddle.height - 20 - ball.radius; // also can assign as ball.y = paddle.y - ball.radius;
    ball.speed_x = 7;
    ball.speed_y = -7; // always upward direction

    vector<Brick> bricks;

    for (int row = 0; row < BRICK_ROWS; row++)
    {
        int hp;
        if (row == 0 || row == 1)
        {
            hp = 3;
        }
        else if (row == 2 || row == 3)
        {
            hp = 2;
        }
        else if (row == 4 || row == 5)
        {
            hp = 1;
        }
        for (int col = 0; col < BRICK_COLS; col++)
        {
            float x = BRICK_OFFSET_LEFT + col * (BRICK_WIDTH + BRICK_PADDING);
            float y = BRICK_OFFSET_TOP + row * (BRICK_HEIGHT + BRICK_PADDING);

            bricks.emplace_back(x, y, BRICK_WIDTH, BRICK_HEIGHT, hp);
        }
    }

    // GameLoop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameState)
        {
        case MENU:
            UpdateMenu();
            DrawMenu();
            break;

        case PLAYING:
            UpdateGame(ball, paddle, bricks);
            DrawGame(ball, paddle, bricks);
            break;

        case PAUSED:
            UpdatePause();
            DrawPause();
            break;

        case GAMEOVER:
            UpdateGameOver(ball, paddle, bricks);
            DrawGameOver();
            break;
        }

        EndDrawing();
    }
    UnloadSound(hitbrick);
    UnloadSound(winsound);
    UnloadSound(gameoversound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}