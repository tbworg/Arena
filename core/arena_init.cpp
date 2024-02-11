#include <raylib.h>
#include <arena.hpp>
#include <player.hpp>
#include <loader.hpp>
#include <iostream>

EngineInstance::EngineInstance(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL); // Invalid key so the window can only be closed by the Game or from the titlebar
    SetTargetFPS(60);     // Will setup settings later

    this->player.position = (Vector2){ 400, 280 };
    this->player.size = (Vector2) { 20, 20};
    this->player.speed = 0;
    this->player.canJump = false;

    this->camera.target = this->player.position; 
    this->camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    this->camera.rotation = 0.0f;
    this->camera.zoom = 1.0f;
}

void EngineInstance::LoadLevel(const char *path) {
    this->env = ArenaLoader::load_level(path);
}

void EngineInstance::ReactiveUpdater() {
    this->camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    this->delta_time = GetFrameTime();
    this->camera.target = this->player.position;

    this->camera.zoom += ((float)GetMouseWheelMove()*0.05f);
    if (this->camera.zoom > 3.0f) { this->camera.zoom = 3.0f; }
    else if (this->camera.zoom < 0.25f) { this->camera.zoom = 0.25f; }
}

void EngineInstance::GameLoop() {
    while (!WindowShouldClose()) { // Wait for exit signal
        this->ReactiveUpdater();
        this->UpdatePlayer();
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(this->camera);
                for (int i = 0; i < this->env.size(); i++) {
                    DrawRectangleRec(this->env[i].rect, this->env[i].color);
                }

                Rectangle playerRect = { player.position.x - (this->player.size.x/2), player.position.y - this->player.size.y, this->player.size.x, this->player.size.y };
                DrawRectangleRec(playerRect, RED);
                
                DrawCircle(player.position.x, player.position.y, 5, GOLD);
            EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}


void EngineInstance::UpdatePlayer() {
    if (IsKeyDown(KEY_A)) { this->player.position.x -= PLAYER_HOR_SPD*this->delta_time; }
    if (IsKeyDown(KEY_D)) { this->player.position.x += PLAYER_HOR_SPD*this->delta_time; }
    if (IsKeyDown(KEY_SPACE) && this->player.canJump) {
        this->player.speed = -PLAYER_JUMP_SPD;
        this->player.canJump = false;
    }

    bool vCollision = false;
    bool hlCollision = false;
    bool hrCollision = false;

    for (int i = 0; i < this->env.size(); i++) {
        if (this->env[i].blocking &&
            CheckCollisionRecs({this->player.position.x, this->player.position.y-this->player.size.y + this->player.speed * this->delta_time,
                                this->player.size.x, this->player.size.y},
                                this->env[i].rect)) {
            vCollision = true;
            this->player.speed = 0.0f;
            this->player.position.y = this->env[i].rect.y;
        }

        if (this->env[i].blocking &&
            CheckCollisionRecs({this->player.position.x - PLAYER_HOR_SPD * this->delta_time, this->player.position.y-this->player.size.y,
                                this->player.size.x, this->player.size.y},
                                this->env[i].rect)) {
            hlCollision = true;
        }

        if (this->env[i].blocking &&
            CheckCollisionRecs({this->player.position.x + PLAYER_HOR_SPD * this->delta_time, this->player.position.y-this->player.size.y,
                                this->player.size.x, this->player.size.y},
                                this->env[i].rect)) {
            hrCollision = true;
        }
    }


    if (hlCollision) {
        this->player.position.x += PLAYER_HOR_SPD*this->delta_time;
    }
    if (hrCollision) {
        this->player.position.x -= PLAYER_HOR_SPD*this->delta_time;
    }
    if (!vCollision) {
        this->player.position.y += this->player.speed*this->delta_time;
        this->player.speed += GRAVITY*this->delta_time;
        this->player.canJump = false;
    }
    else { 
        this->player.canJump = true;
    }
}

namespace Arena {
    EngineInstance *NewInstance(int width, int height, const char *title) {
        EngineInstance *instance = new EngineInstance(width, height, title);
        return instance;
    }
}