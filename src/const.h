
#ifndef CONST_H
#define CONST_H

#define TITLE "Delivery Boys"
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1200
#define FPS_LIMIT 144

#define getTime glfwGetTime

extern double glfwGetTime();

// player constants
#define PLAYER_HEIGHT (70)
#define PLAYER_WIDTH (PLAYER_HEIGHT / 4)
#define DEFAULT_MOVE_TIME (0.15f)
#define PLAYER_START_SPEED (300.0f)
#define PLAYER_ACCL (0.07f)                    // might change this to have speed increase discretely (ie 'level up')

// obstacle constants
#define OBST_TYPE (1)
#define OB_WIDTH (90)
#define OB_HEIGHT (120)

// projectile constants
#define PROJ_TYPE (3)
#define PROJ_WIDTH (40.0f)
#define PROJ_HEIGHT (40.0f)
#define THROW_SPEED (900.0f)
#define PROJ_ROT_SPEED (9.0f)

// target constants
#define TARG_TYPE (4)
#define TARG_WIDTH (50)
#define TARG_HEIGHT (250)

// event constants
#define EVENT_TYPE (2)

// lane constants
#define LEFT_LANE (-1)
#define MID_LANE (0)
#define RIGHT_LANE (1)
#define LANE_DIST (150.0f)
#define LEFT_LANE_POS (SCREEN_WIDTH / 2 - LANE_DIST)
#define MID_LANE_POS (SCREEN_WIDTH / 2)
#define RIGHT_LANE_POS (SCREEN_WIDTH / 2 + LANE_DIST)

#endif