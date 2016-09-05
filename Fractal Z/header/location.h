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

	QDoubleSpinBox* ox, * oy;
	QDoubleSpinBox* sm, * sz;
	QDoubleSpinBox* zd;

	QLabel* originLabel;
	QLabel* speedLabel;
	QLabel* zoomLabel;

	QGroupBox* box;
	QVBoxLayout* layout;
};