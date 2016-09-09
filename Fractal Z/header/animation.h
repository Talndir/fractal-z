#pragma once

#include <vector>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>

#include "../header/location.h"

class Animation : public QObject
{
	Q_OBJECT

public:
	Animation();
	~Animation();

	void init();
	bool nextFrame();
	void reset();

	std::vector<Location> locations;
	Location* keyframe;
	vec2 halfDelta;
	vec2 deltaRemaining;
	double moveMargin;
	double zoomMargin;
	float prevZoom;
	int prevIt;
	float* zoom;
	vec2* origin;
	int* maxIterations;
	unsigned int index;
	bool next;
	int frame;
	QString file, file2;

	QPushButton* buttonAdd;
	QPushButton* buttonDelete;
	QCheckBox* checkBox;

public slots:
	void addLoc();
	void deleteLoc();
	void getFileName(int state);
};