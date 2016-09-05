#pragma once

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtCore/QTextStream>

#include "../header/collapsiblepanelwidget.h"
#include "../header/resolver.h"

extern int WINDOW_WIDTH;															// Window width in pixels
extern int WINDOW_HEIGHT;															// Window height in pixels
extern float RATIO;																	// Aspect ratio (16:9)
extern int BLOCK_WIDTH;																// Width of each block
extern int BLOCK_HEIGHT;															// Height of each block
#define BLOCKS_HORIZONTAL (WINDOW_WIDTH / BLOCK_WIDTH)								// Number of blocks horizontally
#define BLOCKS_VERTICAL (WINDOW_HEIGHT / BLOCK_HEIGHT)								// Number of blocks vertically
#define BLOCKS_EXTRA_HORIZONTAL 1													// Extra horizontal buffer blocks
#define BLOCKS_EXTRA_VERTICAL 1														// Extra vertical buffer blocks
#define BLOCKS_TOTAL_HORIZONTAL (BLOCKS_HORIZONTAL + (2 * BLOCKS_EXTRA_HORIZONTAL))	// Total horizontal blocks
#define BLOCKS_TOTAL_VERTICAL (BLOCKS_VERTICAL + (2 * BLOCKS_EXTRA_VERTICAL))		// Total vertical blocks
extern int IMAGE_WIDTH;																// Width of entire render area
extern int IMAGE_HEIGHT;															// Height of entire render area
#define PIXEL_MOVE 16																// Pixels moved per 'step'
#define ORIGIN_MOVE ((float(PIXEL_MOVE) / float(WINDOW_HEIGHT)) * 2)				// Origin movement per 'step'

enum ERENDERMODE { NONE, ALL, LEFT, RIGHT, TOP, BOTTOM };
extern ERENDERMODE rendermodeLR;
extern ERENDERMODE rendermodeTB;

extern bool keys[1024];

extern QVector2D renderOffset;
extern QVector2D offset;
extern QVector2D majorOffset;
extern vec2 origin;
extern float zoom;
extern int maxIterations;

extern bool autoIterations;

extern CollapsiblePanelWidget* pane;
extern QVBoxLayout* paneLayout;
extern QGroupBox* paneBox;

extern CollapsiblePanelWidget* anim;
extern QVBoxLayout* animLayout;
extern QGroupBox* animBox;

extern Resolver* r;