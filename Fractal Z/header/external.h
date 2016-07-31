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
extern int mode;					// 1 = Mandelbrot, 2 = Julia, 3 = Newton, 4 = Collatz
extern glm::ivec2 offset;
extern glm::ivec2 majorOffset;
enum ERENDERMODE { NONE, ALL, LEFT, RIGHT, TOP, BOTTOM };
extern ERENDERMODE rendermodeLR;
extern ERENDERMODE rendermodeTB;
extern bool bRenderAllNow;
extern bool bChangeColour;
extern float colourOffset;
extern float colourScale;