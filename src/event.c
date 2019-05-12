
#include "event.h"

// EVENT: Random Objects
#define MIN_OB_SPAWN_DIST (PLAYER_HEIGHT * 2 + OB_HEIGHT)
#define MAX_OB_SPAWN_DIST (OB_HEIGHT * 3)
#define MIN_TARG_SPAWN_DIST (TARG_HEIGHT * 1.5)
#define MAX_TARG_SPAWN_DIST (TARG_HEIGHT * 5)
// randomly spawn obstacles event
void initRandObs(struct Event *e);
void updateRandObs(struct Event *e, struct List *obs, struct List *targs);
void destroyRandObs(struct Event *e);

struct RandObsParams {
    float next_ob_spawn_dist;
    float next_targ_spawn_dist;
} rand_obs_params;

struct Event rand_obs = {
    .total_dist = 3000,
    .init = initRandObs,
    .update = updateRandObs,
    .destroy = destroyRandObs
};

// ************ Public Functions *************

void initEvent(struct Event *e, struct TexMan *texman) {
    float current_time = getTime();
    e->last_update_time = current_time;
    e->last_speed = 0;
    e->current_dist = 0;
    e->texman = texman;
    e->init(e);
}

int updateEvent(struct Event *e, struct Player *p, struct List *obs, struct List *targs) {
    float current_time = getTime();
    float dt = current_time - e->last_update_time;
    e->last_update_time = current_time;
    e->current_dist += e->last_speed * dt;
    if(e->current_dist > e->total_dist) {
        // done with this event
        return 1;
    }
    e->last_speed = p->speed;
    e->update(e, obs, targs);

    return 0;
}

void destroyEvent(struct Event *e) {
    e->destroy(e);
}

// ********** Private Functions ***********

void initRandObs(struct Event *e) {
    e->params = malloc(sizeof(struct RandObsParams));
    ((struct RandObsParams *)(e->params))->next_ob_spawn_dist = MIN_OB_SPAWN_DIST;
    ((struct RandObsParams *)(e->params))->next_targ_spawn_dist = MIN_TARG_SPAWN_DIST;
}

void updateRandObs(struct Event *e, struct List *obs, struct List *targs) {
    struct RandObsParams *p = (struct RandObsParams *)e->params;

    // if object should be spawned
    if(p->next_ob_spawn_dist < e->current_dist) {
        // spawn a single obstacle in a random lane
        int lane = (rand() % 3)  - 1;
        addObstacle(obs, lane, e->texman);
        p->next_ob_spawn_dist = rand() % (MAX_OB_SPAWN_DIST - MIN_OB_SPAWN_DIST) + MIN_OB_SPAWN_DIST + e->current_dist;
    }

    if(p->next_targ_spawn_dist < e->current_dist) {
        int side = (rand() % 2) - 1;
        addTarget(targs, side, e->texman);
        p->next_targ_spawn_dist = rand() % (int)(MAX_TARG_SPAWN_DIST - MIN_TARG_SPAWN_DIST) + MIN_TARG_SPAWN_DIST + e->current_dist;
    }
}

void destroyRandObs(struct Event *e) {
    free(e->params);
}

