#include <raylib.h>

int main() {
    InitWindow(300, 600, "raylib - tetris");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
