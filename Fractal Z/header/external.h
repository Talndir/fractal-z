// ===============================================================
/*
extern.h

Holds all external variables (global, defined in main.cpp, declared wherever this header is included)
*/

// ===============================================================
#pragma once

// ===============================================================
/* Includes */
#include <vector>	// Vectors for keystates

#include <GLM\glm.hpp>

// ===============================================================
/* The variables */
extern std::vector<bool> keys;		// Holds state for all keys (1 = pressed, 0 = not pressed)
extern float zoom;
extern glm::vec2 origin;
extern int mode;					// 1 = Mandelbrot, 2 = Julia