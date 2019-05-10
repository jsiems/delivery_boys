
#include "game.h"

// ********* Constants ************
#define PLAYER_WIDTH (100)

#define LEFT_LANE (-1)
#define MID_LANE (0)
#define RIGHT_LANE (1)
#define LANE_DIST 150.0f

#define LEFT_LANE_POS (SCREEN_WIDTH / 2 - LANE_DIST)
#define MID_LANE_POS (SCREEN_WIDTH / 2)
#define RIGHT_LANE_POS (SCREEN_WIDTH / 2 + LANE_DIST)

#define DEFAULT_MOVE_TIME (0.25f)


// keeps track of player location
struct Player {
    float last_update_time;

    int tex_id;
    float x;
    float y;
    int moving;                 // if player is currently moving
    int l_move_processed;       // flag indicating if current key press has been processed yet
    int r_move_processed;
    float move_start_time;      // time move command was started
    float move_time;            // based on how far from destination player is, entire lanes distance is DEFAULT_MOVE_TIME
    float start_pos;            // starting x position of player
    float end_pos;              // where the player should end up

    int current_lane;

    int left, right;  // what keys are currently pressed
};



// *********** global variables **************
struct Shader sprite_shader_g;
struct TexMan texman_g;
struct SpriteRenderer sprite_g;

// player variables
struct Player player_g;

// *********** Prototypes *********
void drawPlayer(struct Player *p);

// returns the x coordinate of the input lane position as a float
float getLanePos(int lane);



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

    // load textures
    player_g.last_update_time = time();
    player_g.x = SCREEN_WIDTH / 2;
    player_g.y = 5 * SCREEN_HEIGHT / 6;

    player_g.tex_id = getTextureId(&texman_g, "circle");

    player_g.current_lane = MID_LANE;
    player_g.moving = 0;
    player_g.l_move_processed = 0;
    player_g.r_move_processed = 0;

    player_g.left = 0;
    player_g.right = 0;
}

void updateGame() {
    // update stuff

    // intialize some default uniforms
    mat4 view, projection;
    glm_translate_make(view, (vec3){0.0f, 0.0f, 0.0f});
    glm_ortho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0, projection);
    setMat4(&sprite_shader_g, "view", view);
    setMat4(&sprite_shader_g, "projection", projection);

    float dt = time() - player_g.last_update_time;
    player_g.last_update_time = time();

    // check for inputs
    if(player_g.left && !player_g.l_move_processed && player_g.current_lane != LEFT_LANE) {
        player_g.current_lane --;
        player_g.moving = 1;
        player_g.start_pos = player_g.x;
        player_g.end_pos = getLanePos(player_g.current_lane);
        player_g.move_start_time = time();
        player_g.move_time = DEFAULT_MOVE_TIME * abs(player_g.end_pos - player_g.start_pos) / (LANE_DIST);
        player_g.l_move_processed = 1;
    }
    if(player_g.right && !player_g.r_move_processed && player_g.current_lane != RIGHT_LANE) {
        player_g.current_lane ++;
        player_g.moving = 1;
        player_g.start_pos = player_g.x;
        player_g.end_pos = getLanePos(player_g.current_lane);
        player_g.move_start_time = time();
        player_g.move_time = DEFAULT_MOVE_TIME * abs(player_g.end_pos - player_g.start_pos) / (LANE_DIST);
        player_g.r_move_processed = 1;
    }

    // clear flag indicating that the current key press was proccessed
    // after the player released the key
    if(!player_g.left && player_g.l_move_processed) {
        player_g.l_move_processed = 0;
    }
    if(!player_g.right && player_g.r_move_processed) {
        player_g.r_move_processed = 0;
    }

    // if the player is moving
    if(player_g.moving && time() < player_g.move_start_time + player_g.move_time) {
        player_g.x = player_g.start_pos + (player_g.end_pos - player_g.start_pos) * 
                        SineEaseOut((time() - player_g.move_start_time)/ (player_g.move_time));
    }
    // if player has finished moving
    else if(player_g.moving) {
        player_g.x = player_g.end_pos;
        player_g.moving = 0;
    }

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
}



// *************** Private Functions *******************

void drawPlayer(struct Player * p) {
    drawSprite(&sprite_g, &sprite_shader_g, p->tex_id, 
    (vec2){p->x - PLAYER_WIDTH / 2, p->y - PLAYER_WIDTH / 2},                         // position
    (vec2) {PLAYER_WIDTH, PLAYER_WIDTH},        // size
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
