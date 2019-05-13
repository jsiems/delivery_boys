
#include "game.h"

// *********** global variables **************
struct Shader sprite_shader_g;
struct TexMan texman_g;
struct SpriteRenderer sprite_g;

// player variables
struct Player player_g;

// obstacles on screen
struct List obstacles;

// events
struct List events;
struct Event *current_event = 0;

// projectiles
struct List projs;

// targets
struct List targs;



// *********** Prototypes *********
// resets delivery boys game
void resetGame();



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

    // intialize some default uniforms
    mat4 view, projection;
    glm_translate_make(view, (vec3){0.0f, 0.0f, 0.0f});
    glm_ortho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0, projection);
    setMat4(&sprite_shader_g, "view", view);
    setMat4(&sprite_shader_g, "projection", projection);

    // initialize the texture manager
    initTexMan(&texman_g);

    // initialize sprite renderer
    initSpriteRenderer(&sprite_g);

    // initialize player
    initPlayer(&player_g, &texman_g);

    // initialize obstacles list
    initList(&obstacles);
    initList(&events);
    insertNode(&events, &rand_obs, sizeof(struct Event), EVENT_TYPE);

    initList(&projs);

    initList(&targs);

    initTextRenderer();
}

int updateGame() {
    // update stuff
    updatePlayer(&player_g);

    updateProjs(&projs, &player_g, &targs, &sprite_g, &sprite_shader_g, &texman_g);

    updateTargets(&targs, &player_g, &sprite_g, &sprite_shader_g);

    int death = updateObstacles(&player_g, &obstacles, &sprite_g, &sprite_shader_g);

    drawPlayer(&player_g, &sprite_g, &sprite_shader_g);

    if(current_event == 0) {
        // find random event
        current_event = (struct Event *)events.front->data;
        initEvent(current_event, &texman_g);
    }
    if(updateEvent(current_event, &player_g, &obstacles, &targs)) {
        destroyEvent(current_event);
        current_event = 0;
    }

    if(death) {
        printf("score: %d\n", player_g.score);
        fflush(stdout);

        resetGame();
    }

    char score_string[50];
    sprintf(score_string, "score: %d", player_g.score);
    renderText(score_string, 10.0f, 30.0f, 0.5f, (vec3){0.2f, 0.8f, 0.4f});

    return 0;
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
    if(key == GLFW_KEY_Z) {
        if(action == GLFW_PRESS) {
            player_g.throw_left = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.throw_left = 0;
        }
    }
    if(key == GLFW_KEY_X) {
        if(action == GLFW_PRESS) {
            player_g.throw_right = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.throw_right = 0;
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
void resetGame() {
    destroyList(&obstacles);
    initList(&obstacles);
    current_event = 0;
    initPlayer(&player_g, &texman_g);
}
