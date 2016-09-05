#pragma once

#include <vector>

#include <QtWidgets/QPushButton>

#include "../header/location.h"

class Animation : public QObject
{
	Q_OBJECT

public:
	Animation();
	~Animation();

	void init();
	bool nextFrame();

	std::vector<Location> locations;
	Location* keyframe;
	vec2 halfDelta;
	vec2 deltaRemaining;
	double moveMargin;
	double zoomMargin;
	float prevZoom;
	float* zoom;
	vec2* origin;
	unsigned int index;
	bool next;

	QPushButton* buttonAdd;
	QPushButton* buttonDelete;

public slots:
	void addLoc();
	void deleteLoc();
};