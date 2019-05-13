// File: func.h
// desc: helper functions any file can use

#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>

#include "const.h"

#define min(x, y) (x < y ? x : y)
#define max(x, y) (!min(x, y))
#define clamp(a, min, max) ((a < min ? min : a) == (a > max ? max : a) ? a : (a < min ? min : max))
#define sign(a) ((a > 0) - (a < 0))

// returns the x coordinate of the input lane position as a float
float getLanePos(int lane);

#endif