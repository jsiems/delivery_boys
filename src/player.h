
#ifndef PLAYER_H
#define PLAYER_H

#include "const.h"
#include "func.h"
#include "render/sprite.h"
#include "render/shader.h"
#include "render/texman.h"
#include "phys.h"
#include "helper/easing.h"

// keeps track of player variables
struct Player {
    float last_update_time;
    int tex_id;
    int score;

    // physics variables
    float x;
    float y;
    struct Circle col_top;
    struct Circle col_bot;     // collision circles
    float speed;

    // throwing states
    int l_throw_proc;
    int r_throw_proc;

    // movement states
    int moving;                 // if player is currently moving
    int l_move_processed;       // flag indicating if current key press has been processed yet
    int r_move_processed;
    float move_start_time;      // time move command was started
    float move_time;            // based on how far from destination player is, entire lanes distance is DEFAULT_MOVE_TIME
    float start_pos;            // starting x position of player
    float end_pos;              // where the player should end up
    int current_lane;

    // inputs (could move to bitmap thing or enum)
    int left, right;  // what keys are currently pressed
    int throw_left, throw_right;
};

// draws player to screen
void drawPlayer(struct Player *p, struct SpriteRenderer *sprite, struct Shader *shader);

// initializes player
void initPlayer(struct Player *p, struct TexMan *texman);

// updates player variables
void updatePlayer(struct Player *p);


#endif
