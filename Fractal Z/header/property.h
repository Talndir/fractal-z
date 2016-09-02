#pragma once

#include <QtWidgets/QSlider>
#include <QtWidgets/QDoubleSpinBox>

class Property : public QObject
{
	Q_OBJECT

public:
	Property(double min, double max, double pos, QWidget* parent);
	~Property();

	double getValue();
	void setValue(double v);

	QSlider* slider;
	double scalar = std::pow(10, 6);

	QDoubleSpinBox* spinbox;

public slots:
	void useSlider(int action);
	void useSpinbox();
};