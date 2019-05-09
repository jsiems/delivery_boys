

#include "game.h"

// keeps track of player location
struct Player {
    float last_update_time;

    int tex_id;
    float x;
    float y;
    float xv;
    float yv;

    int up, down, left, right;
};

// *********** global variables **************
struct Shader sprite_shader_g;
struct TexMan texman_g;
struct SpriteRenderer sprite_g;

// player variables
struct Player player_g;
float accl = 1000;        // 10 p/s/s acceleration
float airres = 0.000000000000001;    // 1% air resistance


// *********** Prototypes *********
void drawPlayer(struct Player *p);



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
    player_g.y = SCREEN_HEIGHT / 2;
    player_g.xv = 0;
    player_g.yv = 0;
    player_g.tex_id = getTextureId(&texman_g, "circle");
    player_g.up = 0;
    player_g.down = 0;
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
    if(player_g.up) {
        player_g.yv -= accl * dt;
    }
    if(player_g.down) {
        player_g.yv += accl * dt;
    }
    if(player_g.left) {
        player_g.xv -= accl * dt;
    }
    if(player_g.right) {
        player_g.xv += accl * dt;
    }
    //player_g.yv *= (1 - airres) * dt;
    //player_g.xv *= (1 - airres) * dt;

    player_g.x += player_g.xv * dt;
    player_g.y += player_g.yv * dt;

    drawPlayer(&player_g);
}

// Input Processing
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // quit game on escape press
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1U);
    }


    if(key == GLFW_KEY_UP) {
        if(action == GLFW_PRESS) {
            player_g.up = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.up = 0;
        }
    }
    if(key == GLFW_KEY_DOWN) {
        if(action == GLFW_PRESS) {
            player_g.down = 1;
        }
        else if(action == GLFW_RELEASE) {
            player_g.down = 0;
        }
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
    (vec2){p->x, p->y},             // position
    (vec2) {100.0f, 100.0f},        // size
    0.0f,                           // rotation
    (vec3){1.0f, 1.0f, 1.0f});      // color
}

