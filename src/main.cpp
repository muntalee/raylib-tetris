#include "colors.hpp"
#include "game.hpp"
#include "raylib.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <grid.hpp>
#include <iostream>

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

        DrawTextEx(font, "Score", {360, 25}, 32, 2, WHITE);
        DrawRectangle(320, 70, 170, 65, darkGrey);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 32, 2);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 85}, 32, 2,
                   WHITE);

        DrawTextEx(font, "Next", {370, 185}, 32, 2, WHITE);
        DrawRectangle(320, 230, 170, 170, darkGrey);

        if (game.gameOver) {
            DrawTextEx(font, "GAME\nOVER", {370, 455}, 32, 2, WHITE);
        }

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
