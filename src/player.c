
#include "player.h"

void drawPlayer(struct Player * p, struct SpriteRenderer *sprite, struct Shader *shader) {

    // top collision circle
    drawSprite(sprite, shader, p->tex_id, 
    (vec2){p->col_top.pos.x - p->col_top.radius, p->col_top.pos.y - p->col_top.radius},   // position
    (vec2) {p->col_top.radius * 2, p->col_top.radius * 2},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});                  // color

    // bottom collision circle
    drawSprite(sprite, shader, p->tex_id, 
    (vec2){p->col_bot.pos.x - p->col_bot.radius, p->col_bot.pos.y - p->col_bot.radius},   // position
    (vec2) {p->col_bot.radius * 2, p->col_bot.radius * 2},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});                  // color
}

void initPlayer(struct Player *p, struct TexMan *texman) {
    // physics variables
    p->last_update_time = getTime();
    p->score = 0;
    p->x = SCREEN_WIDTH / 2;
    p->y = 5 * SCREEN_HEIGHT / 6;
    p->col_top.pos.x = p->x;
    p->col_top.pos.y = p->y - PLAYER_HEIGHT / 4;
    p->col_top.radius = PLAYER_HEIGHT / 4;
    p->col_bot.pos.x = p->x;
    p->col_bot.pos.y = p->y + PLAYER_HEIGHT / 4;
    p->col_bot.radius = PLAYER_HEIGHT / 4;
    p->speed = PLAYER_START_SPEED;

    p->tex_id = getTextureId(texman, "circle");

    // lane movement stuff
    p->current_lane = MID_LANE;
    p->moving = 0;
    p->l_move_processed = 0;
    p->r_move_processed = 0;

    p->left = 0;
    p->right = 0;
}

void updatePlayer(struct Player *p) {
    // check for inputs
    if(p->left && !p->l_move_processed && p->current_lane != LEFT_LANE) {
        p->current_lane --;
        p->moving = 1;
        p->start_pos = p->x;
        p->end_pos = getLanePos(p->current_lane);
        p->move_start_time = getTime();
        p->move_time = DEFAULT_MOVE_TIME * abs(p->end_pos - p->start_pos) / (LANE_DIST);
        p->l_move_processed = 1;
    }
    if(p->right && !p->r_move_processed && p->current_lane != RIGHT_LANE) {
        p->current_lane ++;
        p->moving = 1;
        p->start_pos = p->x;
        p->end_pos = getLanePos(p->current_lane);
        p->move_start_time = getTime();
        p->move_time = DEFAULT_MOVE_TIME * abs(p->end_pos - p->start_pos) / (LANE_DIST);
        p->r_move_processed = 1;
    }

    // clear flag indicating that the current key press was proccessed
    // after the player released the key
    if(!p->left && p->l_move_processed) {
        p->l_move_processed = 0;
    }
    if(!p->right && p->r_move_processed) {
        p->r_move_processed = 0;
    }

    // if the player is moving
    if(p->moving && getTime() < p->move_start_time + p->move_time) {
        p->x = p->start_pos + (p->end_pos - p->start_pos) * 
                        SineEaseOut((getTime() - p->move_start_time)/ (p->move_time));
    }
    // if player has finished moving
    else if(p->moving) {
        p->x = p->end_pos;
        p->moving = 0;
    }

    // update collision circle locations
    p->col_top.pos.x = p->x;
    p->col_bot.pos.x = p->x;

    // update speed
    p->speed += PLAYER_ACCL;
}

