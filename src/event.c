
#include "event.h"

// EVENT: Random Objects

#define MIN_SPAWN_DIST (PLAYER_HEIGHT * 2 + OB_HEIGHT)
#define MAX_SPAWN_DIST (OB_HEIGHT * 3)
// randomly spawn obstacles event
void initRandObs(struct Event *e);
void updateRandObs(struct Event *e, struct List *obs);
void destroyRandObs(struct Event *e);

struct RandObsParams {
    float next_spawn_dist;
} rand_obs_params;

struct Event rand_obs = {
    .total_dist = 1000,
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

int updateEvent(struct Event *e, struct Player *p, struct List *obs) {
    float current_time = getTime();
    float dt = current_time - e->last_update_time;
    e->last_update_time = current_time;
    e->current_dist += e->last_speed * dt;
    if(e->current_dist > e->total_dist) {
        // done with this event
        return 1;
    }
    e->last_speed = p->speed;
    e->update(e, obs);

    return 0;
}

void destroyEvent(struct Event *e) {
    e->destroy(e);
}

// ********** Private Functions ***********

void initRandObs(struct Event *e) {
    e->params = malloc(sizeof(struct RandObsParams));
    ((struct RandObsParams *)(e->params))->next_spawn_dist = MIN_SPAWN_DIST;
}

void updateRandObs(struct Event *e, struct List *obs) {
    struct RandObsParams *p = (struct RandObsParams *)e->params;

    // if object should be spawned
    if(p->next_spawn_dist < e->current_dist) {
        // spawn a single obstacle in a random lane
        int lane = (rand() % 3)  - 1;
        addObstacle(obs, lane, e->texman);
        p->next_spawn_dist = rand() % (MAX_SPAWN_DIST - MIN_SPAWN_DIST) + MIN_SPAWN_DIST + e->current_dist;
    }
}

void destroyRandObs(struct Event *e) {
    free(e->params);
}

