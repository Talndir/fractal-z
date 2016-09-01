#include "../header/property.h"
#include <QtCore/QTextStream>
Property::Property(float min, float max, float pos, QWidget* parent)
{
	slider = new QSlider();
	slider->setOrientation(Qt::Horizontal);
	slider->setParent(parent);
	slider->setMinimum(min * scalar);
	slider->setMaximum(max * scalar);
	slider->setSliderPosition(pos * scalar);

	connect(slider, SIGNAL(actionTriggered(int)), this, SLOT(useSlider(int)));

	spinbox = new QDoubleSpinBox();
	spinbox->setParent(parent);
	spinbox->setMinimum(min);
	spinbox->setMaximum(max);
	spinbox->setValue(pos);
	spinbox->setDecimals(10);
	spinbox->setSingleStep(0.0000000001);

	connect(spinbox, SIGNAL(editingFinished()), this, SLOT(useSpinbox()));
}

Property::~Property()
{
}

float Property::getValue()
{
	return spinbox->value();
}

void Property::setValue(float v)
{
	slider->setSliderPosition(v * scalar);
	spinbox->setValue(v);
}

void Property::useSlider(int action)
{
	spinbox->setValue(float(slider->value()) / scalar);
}

void Property::useSpinbox()
{
	slider->setSliderPosition(spinbox->value() * scalar);
}