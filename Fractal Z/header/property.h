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
	float scalar = std::pow(10, 6);

	QDoubleSpinBox* spinbox;

public slots:
	void useSlider(int action);
	void useSpinbox();
};