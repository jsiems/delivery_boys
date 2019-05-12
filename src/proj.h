// File: proj.h
// Desc: containts functions for projectiles (newspapers)


#ifndef PROJ_H
#define PROJ_H

#include <math.h>

#include "phys.h"
#include "helper/list.h"
#include "player.h"

struct Proj {
    int tex_id;

    float x;
    float y;
    float xv;
    float rot;
    float rot_speed;

    float last_update_time;

    struct Circle col;
};

// draws project to screen
void drawProj(struct Proj *p, struct SpriteRenderer *sprite, struct Shader *shader);

// adds projectile to list of projectiles
void addProj(struct List *pjs, struct Player *p, struct TexMan *tman, int dir);

// updates projectiles
// will need list of houses soon
void updateProjs(struct List *pjs, struct Player *p, struct SpriteRenderer *sprite, struct Shader *shader);


#endif
