#include "../raylib/include/raylib.h"
#include <player.hpp>
#include <loader.hpp>
#include <vector>
#include <map>

#ifndef ARENA_HPP
#define ARENA_HPP

class EngineInstance {
    public:
        EngineInstance(int width, int height, const char *title);
        void GameLoop();
        void LoadLevel(const char *path);
        void LoadTextures(const char *path);
        void ReactiveUpdater();
        void UpdatePlayer();
        void DrawBG();
        void DrawObj(Object obj);
        void DrawPlayer();
    
    private:
        Player player = { {} };
        Camera2D camera = { {} };
        std::map<int, Texture2D> textures;
        std::map<int, std::string> texture_map;
        Level c_level;
        float delta_time;
};

namespace Arena {
    EngineInstance *NewInstance(int width, int height, const char *title);
}

#endif