#include "../header/animation.h"
#include "../header/external.h"

Animation::Animation()
{
	index = -1;
	next = true;
}

Animation::~Animation()
{
}

void Animation::init()
{
	keyframe = &locations.at(index);
	
	halfDelta = vec2((keyframe->destination.x - origin->x) / 2.0, (keyframe->destination.y - origin->y) / 2.0);
	deltaRemaining = vec2(keyframe->destination.x - origin->x, keyframe->destination.y - origin->y);
	moveMargin = 2.0 / (WINDOW_HEIGHT * keyframe->zoomDest);
	zoomMargin = 0.04;
}

bool Animation::nextFrame()
{
	if (next)
	{
		prevZoom = log2l(*zoom);

		++index;
		if (index >= locations.size())
		{
			--index;
			return false;
		}
		else
			init();
	}

	next = true;

	double z1 = log2l(*zoom);
	double z2 = log2l(keyframe->zoomDest);
	double m = (0.1 - (std::abs(((z1 - prevZoom) / (z2 - prevZoom)) - 0.5) * 0.2)) + keyframe->speedZoom;
	bool in = prevZoom < z2 ? true : false;


	if ((std::abs(deltaRemaining.x) > moveMargin) || (std::abs(deltaRemaining.y) > moveMargin))
	{
		origin->x += deltaRemaining.x - (deltaRemaining.x / keyframe->speedMove);
		origin->y += deltaRemaining.y - (deltaRemaining.y / keyframe->speedMove);

		deltaRemaining = vec2(keyframe->destination.x - origin->x, keyframe->destination.y - origin->y);
		next = false;
	}

	if (!in)
		m = 1 / m;

	if (std::abs(z1 - z2) > zoomMargin)
	{
		*zoom *= m;

		next = false;
	}

	return true;
}