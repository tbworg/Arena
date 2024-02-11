#include "../raylib/include/raylib.h"
#include <player.hpp>
#include <loader.hpp>
#include <vector>

#ifndef ARENA_HPP
#define ARENA_HPP

class EngineInstance {
    public:
        EngineInstance(int width, int height, const char *title);
        void GameLoop();
        void LoadLevel(const char *path);
        void ReactiveUpdater();
        void UpdatePlayer();
    
    private:
        Player player = { {} };
        Camera2D camera = { {} };
        std::vector<Object> env;
        float delta_time;
};

namespace Arena {
    EngineInstance *NewInstance(int width, int height, const char *title);
}

#endif