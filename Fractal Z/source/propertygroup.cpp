#include "../header/propertygroup.h"

PropertyGroup::PropertyGroup(float min, float max, float t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min, max, t, box));

	config();
}

PropertyGroup::PropertyGroup(int min, int max, int t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min, max, t, box));

	properties.at(0)->spinbox->setDecimals(0);
	properties.at(0)->spinbox->setSingleStep(1);

	config();
}

PropertyGroup::PropertyGroup(QVector2D min, QVector2D max, QVector2D t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min.x(), max.x(), t.x(), box));
	properties.emplace_back(new Property(min.y(), max.y(), t.y(), box));

	config();
}

PropertyGroup::PropertyGroup(QVector3D min, QVector3D max, QVector3D t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min.x(), max.x(), t.x(), box));
	properties.emplace_back(new Property(min.y(), max.y(), t.y(), box));
	properties.emplace_back(new Property(min.z(), max.z(), t.z(), box));

	config();
}

PropertyGroup::PropertyGroup(QVector4D min, QVector4D max, QVector4D t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min.x(), max.x(), t.x(), box));
	properties.emplace_back(new Property(min.y(), max.y(), t.y(), box));
	properties.emplace_back(new Property(min.z(), max.z(), t.z(), box));
	properties.emplace_back(new Property(min.w(), max.w(), t.w(), box));

	config();
}

PropertyGroup::PropertyGroup(vec2 min, vec2 max, vec2 t, QString name)
{
	createBox(name);

	properties.emplace_back(new Property(min.x, max.x, t.x, box));
	properties.emplace_back(new Property(min.y, max.y, t.y, box));

	config();
}

PropertyGroup::~PropertyGroup()
{
}

void PropertyGroup::createBox(QString name)
{
	box = new QGroupBox(name, pane);
}

void PropertyGroup::config()
{
	box->setAlignment(Qt::AlignCenter);
	box->setStyleSheet("border-color:black");
	box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	boxLayout = new QVBoxLayout();

	for (unsigned int i = 0; i < properties.size(); ++i)
	{
		boxLayout->addWidget(properties.at(i)->slider);
		boxLayout->addWidget(properties.at(i)->spinbox);
	}

	box->setLayout(boxLayout);
	paneLayout->addWidget(box);
}

void PropertyGroup::getValue(float & v)
{
	v = properties.at(0)->getValue();
}

void PropertyGroup::getValue(int & v)
{
	v = properties.at(0)->getValue();
}

void PropertyGroup::getValue(QVector2D & v)
{
	v.setX(properties.at(0)->getValue());
	v.setY(properties.at(1)->getValue());
}

void PropertyGroup::getValue(QVector3D & v)
{
	v.setX(properties.at(0)->getValue());
	v.setY(properties.at(1)->getValue());
	v.setZ(properties.at(2)->getValue());
}

void PropertyGroup::getValue(QVector4D & v)
{
	v.setX(properties.at(0)->getValue());
	v.setY(properties.at(1)->getValue());
	v.setZ(properties.at(2)->getValue());
	v.setW(properties.at(3)->getValue());
}

void PropertyGroup::getValue(vec2 & v)
{
	v.x = properties.at(0)->getValue();
	v.y = properties.at(1)->getValue();
}

void PropertyGroup::setValue(float v)
{
	properties.at(0)->setValue(v);
}

void PropertyGroup::setValue(int v)
{
	properties.at(0)->setValue(v);
}

void PropertyGroup::setValue(QVector2D v)
{
	properties.at(0)->setValue(v.x());
	properties.at(1)->setValue(v.y());
}

void PropertyGroup::setValue(QVector3D v)
{
	properties.at(0)->setValue(v.x());
	properties.at(1)->setValue(v.y());
	properties.at(2)->setValue(v.z());
}

void PropertyGroup::setValue(QVector4D v)
{
	properties.at(0)->setValue(v.x());
	properties.at(1)->setValue(v.y());
	properties.at(2)->setValue(v.z());
	properties.at(3)->setValue(v.w());
}

void PropertyGroup::setValue(vec2 v)
{
	properties.at(0)->setValue(v.x);
	properties.at(1)->setValue(v.y);
}
