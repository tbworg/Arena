#include "../raylib/include/raylib.h"
#include <vector>
#include <map>
#include <string>

#ifndef LOADER_HPP
#define LOADER_HPP

typedef struct Object {
    Rectangle rect;
    bool blocking;
    bool texture;
    Color color;
    int texture_id;
} Object;

typedef struct BgObj {
    bool texture;
    Color color;
    int texture_id;
} BgObj;

typedef struct PlayerObj {
    Rectangle pos_size;
    float speed;
    float jump;
    bool texture;
    Color color;
    int texture_id;
} PlayerObj;

typedef struct Level {
    PlayerObj player;
    BgObj background;
    std::vector<Object> objects;
    std::vector<int> textures;
} Level;


namespace ArenaLoader {
    Level load_level(const char *path);
    std::map<int, std::string> load_mapping(const char *path);
}

#endif