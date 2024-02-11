#include "../raylib/include/raylib.h"

#ifndef PLAYER_HPP
#define PLAYER_HPP

#define GRAVITY 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
    bool canJump;
} Player;


#endif