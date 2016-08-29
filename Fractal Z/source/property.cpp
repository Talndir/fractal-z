#include "../header/property.h"

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
	spinbox->setDecimals(6);
	spinbox->setSingleStep(0.000001);

	connect(spinbox, SIGNAL(editingFinished()), this, SLOT(useSpinbox()));
}

Property::~Property()
{
}

float Property::getValue()
{
	if (useSliderValue)
		return float(slider->sliderPosition()) / scalar;
	else
		return spinbox->value();
}

void Property::setValue(float v)
{
	slider->setSliderPosition(v * scalar);
	spinbox->setValue(v);
}

void Property::useSlider(int action)
{
	useSliderValue = true;
	spinbox->setValue(getValue());
}

void Property::useSpinbox()
{
	useSliderValue = false;
	slider->setSliderPosition(getValue() * scalar);
}