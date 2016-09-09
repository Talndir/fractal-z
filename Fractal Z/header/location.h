#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>

#include "../header/vec2.h"
#include "../header/external.h"

class Location
{
public:
	Location();
	~Location();

	void init();

	vec2 destination;
	double speedZoom;
	double speedMove;
	float zoomDest;
	int iterations;

	QDoubleSpinBox* ox, * oy;
	QDoubleSpinBox* sm, * sz;
	QDoubleSpinBox* zd;
	QDoubleSpinBox* it;

	QLabel* originLabel;
	QLabel* speedLabel;
	QLabel* zoomLabel;
	QLabel* itLabel;

	QGroupBox* box;
	QVBoxLayout* layout;
};