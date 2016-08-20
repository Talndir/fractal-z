#pragma once

#include <QtWidgets/QSlider>

class Property
{
public:
	Property(float min, float max, float def, QWidget* parent);
	~Property();

	float getValue();

	QSlider slider;
	float scalar = 1000.f;
};