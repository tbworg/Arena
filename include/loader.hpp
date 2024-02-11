#include "../raylib/include/raylib.h"
#include <vector>

#ifndef LOADER_HPP
#define LOADER_HPP

typedef struct Object {
    Rectangle rect;
    int blocking;
    Color color;
} Object;

namespace ArenaLoader {
    std::vector<Object> load_level(const char *path);
}

#endif