#include <raylib.h>
#include <arena.hpp>

EngineInstance::EngineInstance(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL); // Invalid key so the window can only be closed by the Game or from the titlebar
    SetTargetFPS(60);     // Will setup settings later
}

void EngineInstance::GameLoop() {
    while (!WindowShouldClose()) { // Wait for exit signal
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
}

namespace Arena {
    EngineInstance *NewInstance(int width, int height, const char *title) {
        EngineInstance *instance = new EngineInstance(width, height, title);
        return instance;
    }
}