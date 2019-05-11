
#include "game.h"

// ********* Constants ************
// player constants
#define PLAYER_HEIGHT (80)
#define PLAYER_WIDTH (PLAYER_HEIGHT / 4)
#define DEFAULT_MOVE_TIME (0.25f)
#define PLAYER_START_SPEED (100)

// obstacle constants
#define OBST_TYPE (1)
#define OB_WIDTH (70)
#define OB_HEIGHT (100)

// lane constants
#define LEFT_LANE (-1)
#define MID_LANE (0)
#define RIGHT_LANE (1)
#define LANE_DIST (150.0f)
#define LEFT_LANE_POS (SCREEN_WIDTH / 2 - LANE_DIST)
#define MID_LANE_POS (SCREEN_WIDTH / 2)
#define RIGHT_LANE_POS (SCREEN_WIDTH / 2 + LANE_DIST)


// keeps track of player variables
struct Player {
    float last_update_time;
    int tex_id;

    // physics variables
    float x;
    float y;
    struct Circle col_top;
    struct Circle col_bot;     // collision circles
    float speed;

    // movement states
    int moving;                 // if player is currently moving
    int l_move_processed;       // flag indicating if current key press has been processed yet
    int r_move_processed;
    float move_start_time;      // time move command was started
    float move_time;            // based on how far from destination player is, entire lanes distance is DEFAULT_MOVE_TIME
    float start_pos;            // starting x position of player
    float end_pos;              // where the player should end up
    int current_lane;

    // inputs (could move to bitmap thing or enum)
    int left, right;  // what keys are currently pressed
};

struct Obstacle {
    float x;
    float y;
    int tex_id;
    struct Rect col_box;
    float last_update_time;
};



// *********** global variables **************
struct Shader sprite_shader_g;
struct TexMan texman_g;
struct SpriteRenderer sprite_g;

// player variables
struct Player player_g;

// obstacles on screen
struct List obstacles;


// *********** Prototypes *********
void drawPlayer(struct Player *p);

// returns the x coordinate of the input lane position as a float
float getLanePos(int lane);

// initializes player
void initPlayer(struct Player *p);

// updates player variables
void updatePlayer(struct Player *p);

// draws a single obstacle
void drawObstacle(struct Obstacle *o);

// adds obstacle to global list
void addObstacle(int lane);



//  ********** Public Functions *************
void initGame() {
    // initialize shaders and such

    // initialize default sprite_shader_g
    if(!initializeShader(&sprite_shader_g, "shaders/sprite_vs.glsl", "shaders/sprite_fs.glsl")) {
        printf("Error initializing sprite shaders\n");
        exit(1);
    }

    // tell the sprite_shader_g which texture to use for which uniform
    // TODO: not sure where to put this...
    // used to be in model.init, but only needs to be called once!
    glUseProgram(sprite_shader_g.id);
    setInt(&sprite_shader_g, "image", 0);

    // initialize the texture manager
    initTexMan(&texman_g);

    // initialize sprite renderer
    initSpriteRenderer(&sprite_g);

    // initialize player
    initPlayer(&player_g);

    // initialize obstacles list
    initList(&obstacles);
    addObstacle(0);
}

void updateGame() {
    // update stuff

    // intialize some default uniforms
    mat4 view, projection;
    glm_translate_make(view, (vec3){0.0f, 0.0f, 0.0f});
    glm_ortho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0, projection);
    setMat4(&sprite_shader_g, "view", view);
    setMat4(&sprite_shader_g, "projection", projection);

    updatePlayer(&player_g);

    // update obstacle positions and check for collisions
    struct Node *i = obstacles.front;
    int death = 0;
    while(i != 0) {
        struct Obstacle *ob = (struct Obstacle *)i->data;
        float current_time = time();
        float dt = current_time -  ob->last_update_time;
        ob->last_update_time = current_time;
        ob->y += player_g.speed * dt;
        ob->col_box.pos.y = ob->y - OB_HEIGHT / 2;
        drawObstacle(ob);
        struct Manifold m1, m2;
        m1.a = &player_g.col_top;
        m1.b = &ob->col_box;
        m2.a = &player_g.col_bot;
        m2.b = &ob->col_box;
        if(isCollidingCircVRect(&m1) || isCollidingCircVRect(&m2)) {
            death = 1;
        }

        i = i->next;
    }

    if(death) {
        printf("big fat poo poo\n");
    }
    else {
        printf("no death\n");
    }
    fflush(stdout);

    drawPlayer(&player_g);
}

// Input Processing
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // quit game on escape press
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1U);
    }

    if(key == GLFW_KEY_LEFT) {
        if(action == GLFW_PRESS) {
            player_g.left = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.left = 0;
        }
    }
    if(key == GLFW_KEY_RIGHT) {
        if(action == GLFW_PRESS) {
            player_g.right = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.right = 0;
        }
    }
}

void mouseMoveCallback(GLFWwindow *window, double x, double y) {

}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

}

void destroyGame() {
    destroyTexMan(&texman_g);
    destroyShader(&sprite_shader_g);
    destroyList(&obstacles);
}



// *************** Private Functions *******************

void drawPlayer(struct Player * p) {

    // top collision circle
    drawSprite(&sprite_g, &sprite_shader_g, p->tex_id, 
    (vec2){p->col_top.pos.x - p->col_top.radius, p->col_top.pos.y - p->col_top.radius},   // position
    (vec2) {p->col_top.radius * 2, p->col_top.radius * 2},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});                  // color

    // bottom collision circle
    drawSprite(&sprite_g, &sprite_shader_g, p->tex_id, 
    (vec2){p->col_bot.pos.x - p->col_bot.radius, p->col_bot.pos.y - p->col_bot.radius},   // position
    (vec2) {p->col_bot.radius * 2, p->col_bot.radius * 2},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 1.0f, 1.0f});                  // color
}

float getLanePos(int lane) {
    switch(lane) {
        case LEFT_LANE:
            return LEFT_LANE_POS;
        case RIGHT_LANE:
            return RIGHT_LANE_POS;
        case MID_LANE:
            return MID_LANE_POS;
        default:
            printf("unkown lane position given to get land pos\n");
            break;
    }

    return 0;
}

void initPlayer(struct Player *p) {
    // physics variables
    player_g.last_update_time = time();
    player_g.x = SCREEN_WIDTH / 2;
    player_g.y = 5 * SCREEN_HEIGHT / 6;
    player_g.col_top.pos.x = player_g.x;
    player_g.col_top.pos.y = player_g.y - PLAYER_HEIGHT / 4;
    player_g.col_top.radius = PLAYER_HEIGHT / 4;
    player_g.col_bot.pos.x = player_g.x;
    player_g.col_bot.pos.y = player_g.y + PLAYER_HEIGHT / 4;
    player_g.col_bot.radius = PLAYER_HEIGHT / 4;
    player_g.speed = PLAYER_START_SPEED;

    player_g.tex_id = getTextureId(&texman_g, "circle");

    // lane movement stuff
    player_g.current_lane = MID_LANE;
    player_g.moving = 0;
    player_g.l_move_processed = 0;
    player_g.r_move_processed = 0;

    player_g.left = 0;
    player_g.right = 0;
}

void updatePlayer(struct Player *p) {
    // check for inputs
    if(p->left && !p->l_move_processed && p->current_lane != LEFT_LANE) {
        p->current_lane --;
        p->moving = 1;
        p->start_pos = p->x;
        p->end_pos = getLanePos(p->current_lane);
        p->move_start_time = time();
        p->move_time = DEFAULT_MOVE_TIME * abs(p->end_pos - p->start_pos) / (LANE_DIST);
        p->l_move_processed = 1;
    }
    if(p->right && !p->r_move_processed && p->current_lane != RIGHT_LANE) {
        p->current_lane ++;
        p->moving = 1;
        p->start_pos = p->x;
        p->end_pos = getLanePos(p->current_lane);
        p->move_start_time = time();
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
    if(p->moving && time() < p->move_start_time + p->move_time) {
        p->x = p->start_pos + (p->end_pos - p->start_pos) * 
                        SineEaseOut((time() - p->move_start_time)/ (p->move_time));
    }
    // if player has finished moving
    else if(p->moving) {
        p->x = p->end_pos;
        p->moving = 0;
    }

    // update collision circle locations
    p->col_top.pos.x = p->x;
    p->col_bot.pos.x = p->x;
}

void drawObstacle(struct Obstacle * o) {
    // top collision circle
    drawSprite(&sprite_g, &sprite_shader_g, o->tex_id, 
    (vec2){o->x - OB_WIDTH / 2, o->y - OB_HEIGHT / 2},   // position
    (vec2) {OB_WIDTH, OB_HEIGHT},        // size
    0.0f,                                       // rotation
    (vec3){1.0f, 0.0f, 0.0f}); 
}

void addObstacle(int lane) {
    struct Obstacle o;

    o.x = getLanePos(lane);
    o.y = -1 * OB_HEIGHT;
    o.tex_id = getTextureId(&texman_g, "rect");
    o.col_box.pos.x = o.x - OB_WIDTH / 2;
    o.col_box.pos.y = o.y - OB_HEIGHT / 2;
    o.col_box.length = OB_WIDTH;
    o.col_box.height = OB_HEIGHT;
    o.last_update_time = time();

    insertNode(&obstacles, &o, sizeof(struct Obstacle), OBST_TYPE);
}
