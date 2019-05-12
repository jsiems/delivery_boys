// File: target.h
// Desc: constains functions and struct for working with targets


#ifndef TARGET_H
#define TARGET_H

#include "const.h"
#include "render/sprite.h"
#include "render/shader.h"
#include "render/texman.h"
#include "phys.h"
#include "helper/list.h"
#include "player.h"

// keeps tracks of target parameters
struct Target {
    float x;
    float y;
    int tex_id;
    struct Rect col_box;
    float last_update_time;
};

// draws a single target
void drawTarget(struct Target *t, struct SpriteRenderer *sprite, struct Shader *shader);

// adds target to obs list
void addTarget(struct List *targs, int side, struct TexMan *texman);

// updates all targets in the list
// returns if any targets are colliding with the player
void updateTargets(struct List *targs, struct Player *p, struct SpriteRenderer *sprite, struct Shader *shader);



#endif
