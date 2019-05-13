// FILE: game.h
// DESC: keeps all variables and functions for current game 

#ifndef GAME_H
#define GAME_H

// build in libraries

// other libraries
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// my files
#include "phys.h"
#include "const.h"
#include "render/sprite.h"
#include "event.h"
#include "proj.h"
#include "target.h"
#include "render/text.h"

// intializes game
void initGame();

// updates all game variables
// draws all game objects to screen
int updateGame();

// events triggered by GLFW (keys pressed, etc...)
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseMoveCallback(GLFWwindow *window, double x, double y);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

// called to free memory when program exits
void destroyGame();

#endif