
#include "target.h"

void drawTarget(struct Target *t, struct SpriteRenderer *sprite, struct Shader *shader) {
    // top collision circle
    drawSprite(sprite, shader, t->tex_id, 
    (vec2){t->x - TARG_WIDTH / 2, t->y - TARG_HEIGHT / 2},   // position
    (vec2) {TARG_WIDTH, TARG_HEIGHT},        // size
    0.0f,                                    // rotation
    (vec3){0.0f, 0.0f, 1.0f}); 
}

void addTarget(struct List *targs, int side, struct TexMan *texman) {
    struct Target t;

    if(side == -1) {
        t.x = TARG_WIDTH / 2;
    }
    else {
        t.x = SCREEN_WIDTH - TARG_WIDTH / 2;
    }
    t.y = -1 * TARG_HEIGHT;
    t.tex_id = getTextureId(texman, "rect");
    t.col_box.pos.x = t.x - TARG_WIDTH / 2;
    t.col_box.pos.y = t.y - TARG_HEIGHT / 2;
    t.col_box.length = TARG_WIDTH;
    t.col_box.height = TARG_HEIGHT;
    t.last_update_time = getTime();

    insertNode(targs, &t, sizeof(struct Target), TARG_TYPE);
}

void updateTargets(struct List *targs, struct Player *p, struct SpriteRenderer *sprite, struct Shader *shader) {
    // update obstacle positions and check for collisions
    struct Node *i = targs->front;
    while(i != 0) {
        struct Target *t = (struct Target *)i->data;
        float current_time = getTime();
        float dt = current_time -  t->last_update_time;
        t->last_update_time = current_time;
        t->y += p->speed * dt;
        t->col_box.pos.y = t->y - TARG_HEIGHT / 2;
        drawTarget(t, sprite, shader);

        if(t->y - TARG_HEIGHT / 2 > SCREEN_HEIGHT) {
            struct Node *temp = i;
            i = i->next;
            removeNode(targs, temp);
        }
        else {
            i = i->next;
        }
    }
}

