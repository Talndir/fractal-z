#include "../header/property.h"

Property::Property(float min, float max, float pos, QWidget* parent)
{
	slider.setOrientation(Qt::Horizontal);
	slider.setParent(parent);
	slider.setMinimum(min * scalar);
	slider.setMaximum(max * scalar);
	slider.setSliderPosition(pos * scalar);
}

Property::~Property()
{
}

float Property::getValue()
{
	return float(slider.sliderPosition()) / scalar;
}
