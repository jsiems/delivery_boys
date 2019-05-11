
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <glad/glad.h>              //defines opengl functions, etc
#include <GLFW/glfw3.h>             //used for window and input
#include <cglm/cglm.h>              //used for maths

#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "sprite.h"
#include "texman.h"
#include "phys.h"
#include "list.h"
#include "const.h"
#include "game.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void glfw_error_callback(int code, const char *err_str);
GLFWwindow *initializeWindow();

float delta_time = 0.0f;
float last_frame = 0.0f;

float last_mouse_x = 400;
float last_mouse_y = 300;
uint8_t first_mouse = 1;

uint8_t draw_wireframe = 0;
uint8_t t_pressed = 0;

int main() {
    printf("running!\n");

    //initialize window
    GLFWwindow *window = initializeWindow();
    //load the opengl library
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initiate GLAD\n");
        return -1;
    }
    //enable depth test
    glEnable(GL_DEPTH_TEST);

    long seed = time(0);
    printf("seed: %lu\n", seed);
    srand(seed);

    initGame();

    //keep track of FPS
    uint64_t total_frames = 0;
    float start_time = glfwGetTime();

    //Main loop
    while(!glfwWindowShouldClose(window)) {
        //wait for max FPS limit
        float min_frame_time = (float)(1 / (float)FPS_LIMIT);
        while(glfwGetTime() - last_frame < min_frame_time);

        //update time since last frame
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        total_frames ++;

        // rendering commands
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(updateGame()) {
            glfwSetWindowShouldClose(window, 1U);
        }
        
        // more rendering commands
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    printf("End of program\n\tframes: %I64d\n\tTime: %f\n\tAverage FPS: %f\n", total_frames, glfwGetTime() - start_time, total_frames / (glfwGetTime() - start_time));

    destroyGame();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_error_callback(int code, const char *err_str) {
    printf("GLFW error: \n\tcode: 0x%x\n\t%s\n", code, err_str);
}

GLFWwindow *initializeWindow() {
    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit()) {
        printf("error initializing GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);

    if(window == NULL) {
        printf("Error creating window\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set callbacks
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    return window;
}
