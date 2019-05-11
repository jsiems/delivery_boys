
#include "obstacle.h"

void drawObstacle(struct Obstacle * o, struct SpriteRenderer *sprite, struct Shader *shader) {
    // top collision circle
    drawSprite(sprite, shader, o->tex_id, 
    (vec2){o->x - OB_WIDTH / 2, o->y - OB_HEIGHT / 2},   // position
    (vec2) {OB_WIDTH, OB_HEIGHT},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 0.0f, 0.0f}); 
}

void addObstacle(struct List *obs, int lane, struct TexMan *texman) {
    struct Obstacle o;

    o.x = getLanePos(lane);
    o.y = -1 * OB_HEIGHT;
    o.tex_id = getTextureId(texman, "rect");
    o.col_box.pos.x = o.x - OB_WIDTH / 2;
    o.col_box.pos.y = o.y - OB_HEIGHT / 2;
    o.col_box.length = OB_WIDTH;
    o.col_box.height = OB_HEIGHT;
    o.last_update_time = getTime();

    insertNode(obs, &o, sizeof(struct Obstacle), OBST_TYPE);
}

int updateObstacles(struct Player *p, struct List *obs, struct SpriteRenderer *sprite, struct Shader *shader) {
    // update obstacle positions and check for collisions
    struct Node *i = obs->front;
    int death = 0;
    while(i != 0) {
        struct Obstacle *ob = (struct Obstacle *)i->data;
        float current_time = getTime();
        float dt = current_time -  ob->last_update_time;
        ob->last_update_time = current_time;
        ob->y += p->speed * dt;
        ob->col_box.pos.y = ob->y - OB_HEIGHT / 2;
        drawObstacle(ob, sprite, shader);
        struct Manifold m1, m2;
        m1.a = &p->col_top;
        m1.b = &ob->col_box;
        m2.a = &p->col_bot;
        m2.b = &ob->col_box;
        if(isCollidingCircVRect(&m1) || isCollidingCircVRect(&m2)) {
            death = 1;
        }

        if(ob->y - OB_HEIGHT / 2 > SCREEN_HEIGHT) {
            struct Node *temp = i;
            i = i->next;
            removeNode(obs, temp);
        }
        else {
            i = i->next;
        }
    }

    return death;
}

