
#include "proj.h"

void drawProj(struct Proj *proj, struct SpriteRenderer *sprite, struct Shader *shader) {
    // top collision circle
    drawSprite(sprite, shader, proj->tex_id, 
    (vec2){proj->x - PROJ_WIDTH / 2, proj->y - PROJ_HEIGHT / 2},   // position
    (vec2) {PROJ_WIDTH, PROJ_HEIGHT},                   // size
    proj->rot,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f}); 
}

void addProj(struct List *pjs, struct Player *p, struct TexMan *tman, int dir) {
    struct Proj proj;

    proj.x = p->x + dir * PLAYER_WIDTH / 2;
    proj.y = p->y;
    proj.xv = THROW_SPEED * dir;
    proj.rot = M_PI_2;              // starting angle is pi / 2
    proj.rot_speed = PROJ_ROT_SPEED * dir;
    proj.tex_id = getTextureId(tman, "newspaper");
    proj.col.pos.x = proj.x;
    proj.col.pos.y = proj.y;
    proj.col.radius = PROJ_WIDTH / 2;
    proj.last_update_time = getTime();

    insertNode(pjs, &proj, sizeof(struct Proj), PROJ_TYPE);
}


void updateProjs(struct List *pjs, struct Player *p, struct SpriteRenderer *sprite, struct Shader *shader) {
    struct Node *c;
    struct Proj *proj;

    c = pjs->front;
    while(c != 0) {
        proj = (struct Proj *)c->data;

        float current_time = getTime();
        float dt = current_time - proj->last_update_time;
        proj->last_update_time = current_time;

        // update location
        proj->x += proj->xv * dt;
        //proj->y += p->speed * dt;  dont add speed till after it has hit a target
        proj->rot += proj->rot_speed * dt;

        drawProj(proj, sprite, shader);

        // check if colliding with target

        // check if offscreen, remove
        if(proj->x + PROJ_WIDTH  < 0 || proj->x - PROJ_WIDTH  > SCREEN_WIDTH
        || proj->y + PROJ_HEIGHT < 0 || proj->y - PROJ_HEIGHT > SCREEN_HEIGHT) {
            struct Node *temp = c;
            c = c->next;
            removeNode(pjs, temp);
        }
        else {
            c = c->next;
        }
    }
}

