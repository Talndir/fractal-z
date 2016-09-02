#include "../header/property.h"
#include <QtCore/QTextStream>
Property::Property(double min, double max, double  pos, QWidget* parent)
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
	spinbox->setDecimals(16);
	spinbox->setMinimum(min);
	spinbox->setMaximum(max);
	spinbox->setValue(pos);
	spinbox->setSingleStep(0.0000000001);

	connect(spinbox, SIGNAL(editingFinished()), this, SLOT(useSpinbox()));
}

Property::~Property()
{
}

double Property::getValue()
{
	return spinbox->value();
}

void Property::setValue(double v)
{
	slider->setSliderPosition(v * scalar);
	spinbox->setValue(v);
}

void Property::useSlider(int action)
{
	spinbox->setValue((slider->value()) / scalar);
}

void Property::useSpinbox()
{
	slider->setSliderPosition(spinbox->value() * scalar);
}