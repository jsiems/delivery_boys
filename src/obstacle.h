// File: obstacle.h
// Desc: constains functions and struct for working with obstacles


#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "const.h"
#include "func.h"
#include "render/sprite.h"
#include "render/shader.h"
#include "render/texman.h"
#include "phys.h"
#include "player.h"
#include "helper/list.h"

// keeps tracks of obstacle parameters
struct Obstacle {
    float x;
    float y;
    int tex_id;
    struct Rect col_box;
    float last_update_time;
};

// draws a single obstacle
void drawObstacle(struct Obstacle * o, struct SpriteRenderer *sprite, struct Shader *shader);

// adds obstacle to obs list
void addObstacle(struct List *obs, int lane, struct TexMan *texman);

// updates all obstacles in the list
// returns if any obstacles are colliding with the player
int updateObstacles(struct Player *p, struct List *obs, struct SpriteRenderer *sprite, struct Shader *shader);



#endif
