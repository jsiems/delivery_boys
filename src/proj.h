// File: proj.h
// Desc: containts functions for projectiles (newspapers)


#ifndef PROJ_H
#define PROJ_H

#include <math.h>

#include "phys.h"
#include "helper/list.h"
#include "player.h"
#include "target.h"

struct Proj {
    int tex_id;

    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float rot;
    float rot_speed;

    float last_update_time;

    int collided;
    float collide_time;
    float col_vx;
    float col_vy;
    float col_rot_vel;
    float rest_time;

    struct Circle col;
};

// draws project to screen
void drawProj(struct Proj *p, struct SpriteRenderer *sprite, struct Shader *shader);

// adds projectile to list of projectiles
void addProj(struct List *pjs, struct Player *p, struct TexMan *tman, int dir);

// updates projectiles
// will need list of houses soon
void updateProjs(struct List *pjs, struct Player *p, struct List *targs, struct SpriteRenderer *sprite, struct Shader *shader, struct TexMan *texman);


#endif
