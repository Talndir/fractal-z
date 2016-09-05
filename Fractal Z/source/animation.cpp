#include "../header/animation.h"
#include "../header/external.h"

Animation::Animation()
{
	reset();

	buttonAdd = new QPushButton("+");
	connect(buttonAdd, SIGNAL(pressed()), this, SLOT(addLoc()));

	buttonDelete = new QPushButton("-");
	connect(buttonDelete, SIGNAL(pressed()), this, SLOT(deleteLoc()));

	checkBox = new QCheckBox("Output pictures?");
	checkBox->setChecked(false);
	connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(getFileName(int)));
}

Animation::~Animation()
{
}

void Animation::init()
{
	keyframe = &locations.at(index);

	keyframe->init();
	
	halfDelta = vec2((keyframe->destination.x - origin->x) / 2.0, (keyframe->destination.y - origin->y) / 2.0);
	deltaRemaining = vec2(keyframe->destination.x - origin->x, keyframe->destination.y - origin->y);
	moveMargin = std::max(2.0 / (WINDOW_HEIGHT * keyframe->zoomDest), std::pow(10, -15));
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

	++frame;

	if (checkBox->isChecked())
	{
		file = file2;
		int d = file.lastIndexOf('.');
		file.replace(d, 1, "_" + QString::number(frame) + ".");

		QTextStream(stdout) << file << endl;
	}

	return true;
}

void Animation::reset()
{
	index = -1;
	next = true;
	frame = 0;
}

void Animation::addLoc()
{
	locations.push_back(Location());
}

void Animation::deleteLoc()
{
	if (locations.size())
	{
		animLayout->removeWidget(locations.back().box);
		locations.pop_back();
	}

	if (index == locations.size() - 1)
		reset();
}

void Animation::getFileName(int state)
{
	if (checkBox->isChecked())
		file2 = QFileDialog::getSaveFileName(anim, "Save as...", "video", "PNG (*.png);; BMP (*.bmp);;TIFF (*.tiff *.tif);; JPEG (*.jpg *.jpeg)");
}
