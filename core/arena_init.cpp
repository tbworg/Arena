#include <raylib.h>
#include <arena.hpp>
#include <player.hpp>
#include <loader.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>

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
    float textX = GetScreenWidth()/2 - MeasureText("Loading Level its Textures...", 25) / 2;
    float textY = GetScreenHeight()/2-25;
    int roundedTextX = static_cast<int>(round(textX));
    int roundedTextY = static_cast<int>(round(textY));

    BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Loading Level its Textures...", roundedTextX, roundedTextY, 25, WHITE);
    EndDrawing();


    this->c_level = ArenaLoader::load_level(path);
    if (this->textures.empty()) {}
    for (int i=0;i<this->c_level.textures.size(); i++) { // Load just the used textures
        int tx_id = this->c_level.textures[i];
        std::string path = this->texture_map.at(tx_id);
        Texture txtr = LoadTexture(
            path.c_str()
        );
        this->textures.insert(
            std::pair<int, Texture2D>(
                tx_id,
                txtr
            )
        );
    }

    this->player.position = (Vector2){this->c_level.player.pos_size.x, this->c_level.player.pos_size.y};
    this->player.size = (Vector2){this->c_level.player.pos_size.width, this->c_level.player.pos_size.height};
}

void EngineInstance::LoadTextures(const char *path) {
    float textX = GetScreenWidth()/2 - MeasureText("Mapping Textures...", 25) / 2;
    float textY = GetScreenHeight()/2-25;
    int roundedTextX = static_cast<int>(round(textX));
    int roundedTextY = static_cast<int>(round(textY));


    BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Mapping Textures...", roundedTextX, roundedTextY, 25, WHITE);
    EndDrawing();

    this->texture_map = ArenaLoader::load_mapping(path);
}

void EngineInstance::DrawBG() {
    if (!this->c_level.background.texture) {
        ClearBackground(this->c_level.background.color);
    } else {
        Texture2D texture = this->textures.at(this->c_level.background.texture_id);
        Rectangle sourceRect = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
        
        // Calculate the destination rectangle to cover the entire screen
        Rectangle destRect = {
            0.0f,                // X position
            0.0f,                // Y position
            (float)GetScreenWidth(),  // Width
            (float)GetScreenHeight()  // Height
        };

        // Draw the texture with the calculated destination rectangle
        DrawTexturePro(texture, sourceRect, destRect, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
}

void EngineInstance::DrawObj(Object obj) {
    if (!obj.texture) {
        DrawRectangleRec(obj.rect, obj.color);
    }
    else {
        Texture2D texture = this->textures.at(obj.texture_id);
        DrawTexturePro(texture, { 0.0f, 0.0f, (float)texture.width, (float)texture.height },
            obj.rect, { 0.0f, 0.0f }, 0.0f, WHITE);
    }
}

void EngineInstance::DrawPlayer() {
    Rectangle playerRect = { player.position.x - (this->player.size.x/2), player.position.y - this->player.size.y, this->player.size.x, this->player.size.y };
    if (!this->c_level.player.texture) {
        DrawRectangleRec(playerRect, this->c_level.player.color);
    } else {
        Texture2D texture = this->textures.at(this->c_level.player.texture_id);
        DrawTexturePro(texture, { 0.0f, 0.0f, (float)texture.width, (float)texture.height },
            {
                this->player.position.x, this->player.position.y,
                this->player.position.x+this->player.size.x,
                this->player.position.y+this->player.size.y
            },
            { 0.0f, 0.0f }, 0.0f, WHITE);
    }
    
    float textX = this->player.position.x - MeasureText("Player", 15) / 2;
    float textY = this->player.position.y - this->player.size.y - 15;
    int roundedTextX = static_cast<int>(round(textX));
    int roundedTextY = static_cast<int>(round(textY));
    DrawText("Player", roundedTextX, roundedTextY, 15, DARKGRAY);
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
            this->DrawBG();
            BeginMode2D(this->camera);
                for (int i = 0; i < this->c_level.objects.size(); i++) {
                    this->DrawObj(this->c_level.objects[i]);
                }
                this->DrawPlayer();
            EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}


void EngineInstance::UpdatePlayer() {
    if (IsKeyDown(KEY_A)) { this->player.position.x -= this->c_level.player.speed*this->delta_time; }
    if (IsKeyDown(KEY_D)) { this->player.position.x += this->c_level.player.speed*this->delta_time; }
    if (IsKeyDown(KEY_SPACE) && this->player.canJump) {
        this->player.speed = -this->c_level.player.jump;
        this->player.canJump = false;
    }

    bool vCollision = false;
    bool hlCollision = false;
    bool hrCollision = false;

    for (int i = 0; i < this->c_level.objects.size(); i++) {
        if (this->c_level.objects[i].blocking &&
            CheckCollisionRecs({this->player.position.x, this->player.position.y-this->player.size.y + this->player.speed * this->delta_time,
                                this->player.size.x, this->player.size.y},
                                this->c_level.objects[i].rect)) {
            vCollision = true;
            this->player.speed = 0.0f;
            this->player.position.y = this->c_level.objects[i].rect.y;
        }

        if (this->c_level.objects[i].blocking &&
            CheckCollisionRecs({this->player.position.x - this->c_level.player.speed * this->delta_time, this->player.position.y-this->player.size.y,
                                this->player.size.x, this->player.size.y},
                                this->c_level.objects[i].rect)) {
            hlCollision = true;
        }

        if (this->c_level.objects[i].blocking &&
            CheckCollisionRecs({this->player.position.x + this->c_level.player.speed * this->delta_time, this->player.position.y-this->player.size.y,
                                this->player.size.x, this->player.size.y},
                                this->c_level.objects[i].rect)) {
            hrCollision = true;
        }
    }


    if (hlCollision) {
        this->player.position.x += this->c_level.player.speed*this->delta_time;
    }
    if (hrCollision) {
        this->player.position.x -= this->c_level.player.speed*this->delta_time;
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