// File: event.h
// Desc: contains all function / structs used for events


#ifndef EVENT_H
#define EVENT_H

#include <stdlib.h>

#include "player.h"
#include "obstacle.h"
#include "target.h"

// an event controls when obstacles spawns
struct Event {
    float last_update_time;             // last time this was updated
    float last_speed;                   // keeps track of the last speed - used for calculating distance travelled
    float total_dist;                   // distance (pixels) of this event
    float current_dist;                 // current distance
    struct TexMan *texman;              // texture manager (might not be needed)
    void *params;                       // pointer to state params (if needed)
    void (*init)(struct Event *e);      // init function, ran each time this event is selected
    void (*update)(struct Event *e, struct List *obs, struct List *targs);    // update function
    void (*destroy)(struct Event *e);   // destroy function, frees any malloced memory for the params
};

// events
extern struct Event rand_obs;


// ************ Functions *************
// initializes and event
void initEvent(struct Event *e, struct TexMan *texman);

// updates a single event, calls it's update function
// returns 1 if event is complete
int updateEvent(struct Event *e, struct Player *p, struct List *obs, struct List *targs);

// destroys an event
void destroyEvent(struct Event *e);

#endif