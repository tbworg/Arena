#include "../raylib/include/raylib.h"

#ifndef ARENA_HPP
#define ARENA_HPP

class EngineInstance {
    public:
        EngineInstance(int width, int height, const char *title);
        void GameLoop();
    
    private:
};

namespace Arena {
    EngineInstance *NewInstance(int width, int height, const char *title);
}

#endif