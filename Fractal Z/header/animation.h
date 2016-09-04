#pragma once

#include <vector>

#include "../header/location.h"

class Animation
{
public:
	Animation();
	~Animation();

	void init();
	bool nextFrame();

	std::vector<location> locations;
	location* keyframe;
	vec2 halfDelta;
	vec2 deltaRemaining;
	double moveMargin;
	double zoomMargin;
	float prevZoom;
	float* zoom;
	vec2* origin;
	unsigned int index;
	bool next;
};