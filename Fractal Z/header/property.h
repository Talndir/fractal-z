#pragma once

#include <QtWidgets/QSlider>
#include <QtWidgets/QDoubleSpinBox>

class Property : public QObject
{
	Q_OBJECT

public:
	Property(float min, float max, float def, QWidget* parent);
	~Property();

	float getValue();
	void setValue(float v);

	QSlider* slider;
	float scalar = 1000.f;

	QDoubleSpinBox* spinbox;

	bool useSliderValue = false;

public slots:
	void useSlider(int action);
	void useSpinbox();
};