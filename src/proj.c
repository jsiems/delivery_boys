
#include "proj.h"

int circ_id = 0;

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
    proj.vx = PROJ_THROW_SPEED * dir;
    proj.vy = 0.0f;
    proj.rot = (float)(rand() % (int)(M_PI_2 * 100.0f)) / 100.0f + M_PI_4;              // starting angle is pi / 2 +- pi / 4
    proj.rot_speed = PROJ_ROT_SPEED * dir;
    proj.tex_id = getTextureId(tman, "newspaper");
    proj.col.pos.x = proj.x;
    proj.col.pos.y = proj.y;
    proj.col.radius = (PROJ_WIDTH / 2) - 10.0f;
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
        proj->x += proj->vx * dt;
        proj->y += proj->vy * dt;
        proj->col.pos.x = proj->x;
        proj->col.pos.y = proj->y;
        proj->rot += proj->rot_speed * dt;
        if(proj->collided) {
            // rest_perc = percent of time while slowing down, i.e. 0.25 = slowed down 25%, 1.0 = stopped
            float rest_perc = min(((current_time - proj->collide_time) / (proj->rest_time)), 1.0f);
            proj->vx = proj->col_vx * (1.0f - rest_perc);
            proj->vy = (p->speed - proj->col_vy) * (rest_perc) + proj->col_vy;
            proj->rot_speed = proj->col_rot_vel * (1.0f - rest_perc);
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
                proj->collide_time = current_time;
                proj->vx *= -((float)(rand() % 5) / 5.0f + 0.25f);
                proj->vy = p->speed;
                proj->col_vx = proj->vx;
                proj->col_vy = ((float)(rand() % 100) / 1.0f - 50.0f) + p->speed;
                proj->ax = -2 * proj->vx;
                proj->rot_speed *= -1;
                proj->col_rot_vel = proj->rot_speed;
                proj->rest_time = (float)(rand() % (int)(100 * (PROJ_MAX_REST_TIME - PROJ_MIN_REST_TIME))) / 100.0f + PROJ_MIN_REST_TIME;
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

