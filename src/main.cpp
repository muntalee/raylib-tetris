#include "colors.hpp"
#include "game.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>

double lastUpdateTime = 0.0;
bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main() {
    srand(time(NULL));
    InitWindow(500, 620, "raylib - tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("data/minecraft_font.ttf", 64, 0, 0);

    Game game = Game();

    while (!WindowShouldClose()) {
        game.HandleInput();

        if (EventTriggered(0.2)) {
            game.MoveBlockDown();
        }

        BeginDrawing();

        ClearBackground(DARKBLUE);
        DrawRectangle(10, 10, 300, 600, DARKGRAY);

        DrawTextEx(font, "Score", {360, 15}, 32, 2, WHITE);
        DrawRectangle(320, 60, 170, 65, darkGrey);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 32, 2);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 75}, 32, 2,
                   WHITE);

        DrawTextEx(font, "Next", {370, 135}, 32, 2, WHITE);
        DrawRectangle(320, 180, 170, 170, darkGrey);

        DrawTextEx(font, "Hold", {370, 360}, 32, 2, WHITE);
        DrawRectangle(320, 405, 170, 170, darkGrey);

        game.Draw();

        if (game.gameOver) {
            const char *gameOverText = "GAME OVER";
            Vector2 textSize = MeasureTextEx(font, gameOverText, 32, 2);
            int boxWidth = textSize.x + 40;
            int boxHeight = textSize.y + 40;
            int boxX = 10 + (300 - boxWidth) / 2;
            int boxY = 10 + (600 - boxHeight) / 2;
            DrawRectangle(boxX, boxY, boxWidth, boxHeight, darkGrey);
            DrawRectangle(boxX + 1, boxY + 1, boxWidth - 2, boxHeight - 2, RED);
            DrawTextEx(
                font, gameOverText,
                {10 + (300 - textSize.x) / 2, 10 + (600 - textSize.y) / 2}, 32,
                2, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
