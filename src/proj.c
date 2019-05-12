
#include "proj.h"

int circ_id = 0;

void drawProj(struct Proj *proj, struct SpriteRenderer *sprite, struct Shader *shader) {
    // top collision circle
    drawSprite(sprite, shader, proj->tex_id, 
    (vec2){proj->x - PROJ_WIDTH / 2, proj->y - PROJ_HEIGHT / 2},   // position
    (vec2) {PROJ_WIDTH, PROJ_HEIGHT},                   // size
    proj->rot,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});

    if(circ_id != 0) 
    drawSprite(sprite, shader, circ_id, 
    (vec2){proj->col.pos.x - proj->col.radius, proj->col.pos.y - proj->col.radius},   // position
    (vec2) {proj->col.radius * 2, proj->col.radius * 2},                   // size
    0.0f,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});  
}

void addProj(struct List *pjs, struct Player *p, struct TexMan *tman, int dir) {
    struct Proj proj;

    proj.x = p->x + dir * PLAYER_WIDTH / 2;
    proj.y = p->y;
    proj.xv = THROW_SPEED * dir;
    proj.rot = M_PI_2;              // starting angle is pi / 2 +- pi / 4
    proj.rot_speed = PROJ_ROT_SPEED * dir;
    proj.tex_id = getTextureId(tman, "newspaper");
    proj.col.pos.x = proj.x;
    proj.col.pos.y = proj.y;
    proj.col.radius = PROJ_WIDTH / 2;
    proj.last_update_time = getTime();
    proj.collided = 0;

    circ_id = p->tex_id;

    insertNode(pjs, &proj, sizeof(struct Proj), PROJ_TYPE);
}


void updateProjs(struct List *pjs, struct Player *p, struct List *targs, struct SpriteRenderer *sprite, struct Shader *shader, struct TexMan *texman) {
    struct Node *c;
    struct Proj *proj;

    // check if player is throwing a projectile
    if(p->throw_left && !p->l_throw_proc) {
        p->l_throw_proc = 1;
        addProj(pjs, p, texman, -1);
    }
    else if(p->throw_right && !p->r_throw_proc) {
        p->r_throw_proc = 1;
        addProj(pjs, p, texman, 1);
    }

    if(!p->throw_left && p->l_throw_proc) {
        p->l_throw_proc = 0;
    }
    if(!p->throw_right && p->r_throw_proc) {
        p->r_throw_proc = 0;
    }

    c = pjs->front;
    while(c != 0) {
        proj = (struct Proj *)c->data;

        float current_time = getTime();
        float dt = current_time - proj->last_update_time;
        proj->last_update_time = current_time;

        // update location
        proj->x += proj->xv * dt;
        proj->col.pos.x = proj->x;
        proj->col.pos.y = proj->y;
        proj->rot += proj->rot_speed * dt;
        if(proj->collided) {
            proj->y += p->speed * dt;
            proj->xv *= 0.8 * dt;
            proj->rot_speed *= 0.8 * dt;
        }

        drawProj(proj, sprite, shader);

        // check if colliding with each target, only if it has not collided with anything yet
        struct Node *i;
        i = targs->front;
        while(i != 0 && proj->collided == 0) {
            struct Target *t;
            t = (struct Target *)i->data;
            
            struct Manifold m;
            m.a = &proj->col;
            m.b = &t->col_box;
            if(isCollidingCircVRect(&m)) {
                p->score ++;
                proj->collided = 1;
                proj->xv *= 0;
            }

            i = i->next;
        }

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

