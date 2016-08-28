#pragma once

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>

#include "../header/collapsiblepanelwidget.h"

extern int WINDOW_WIDTH;															// Window width in pixels
extern int WINDOW_HEIGHT;															// Window height in pixels
extern float RATIO;																	// Aspect ratio (16:9)
extern int BLOCK_WIDTH;																// Width of each block
extern int BLOCK_HEIGHT;																// Height of each block
#define BLOCKS_HORIZONTAL (WINDOW_WIDTH / BLOCK_WIDTH)								// Number of blocks horizontally
#define BLOCKS_VERTICAL (WINDOW_HEIGHT / BLOCK_HEIGHT)								// Number of blocks vertically
#define BLOCKS_EXTRA_HORIZONTAL 1													// Extra horizontal buffer blocks
#define BLOCKS_EXTRA_VERTICAL 1														// Extra vertical buffer blocks
#define BLOCKS_TOTAL_HORIZONTAL (BLOCKS_HORIZONTAL + (2 * BLOCKS_EXTRA_HORIZONTAL))	// Total horizontal blocks
#define BLOCKS_TOTAL_VERTICAL (BLOCKS_VERTICAL + (2 * BLOCKS_EXTRA_VERTICAL))		// Total vertical blocks
extern int IMAGE_WIDTH;																// Width of entire render area
extern int IMAGE_HEIGHT;															// Height of entire render area

extern QVector2D renderOffset;
extern QVector2D offset;
extern QVector2D origin;
extern float zoom;

//extern QScrollArea* pane;
extern CollapsiblePanelWidget* pane;
extern QVBoxLayout* paneLayout;
extern QGroupBox* paneBox;