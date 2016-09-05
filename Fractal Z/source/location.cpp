#include "../header/location.h"

Location::Location()
{
	ox = new QDoubleSpinBox();
	ox->setDecimals(20);
	ox->setValue(0.0);
	ox->setRange(-1000.0, 1000.0);
	ox->setSingleStep(std::pow(10, -20));

	oy = new QDoubleSpinBox();
	oy->setDecimals(20);
	oy->setValue(0.0);
	oy->setRange(-1000.0, 1000.0);
	oy->setSingleStep(std::pow(10, -20));

	sm = new QDoubleSpinBox();
	sm->setDecimals(8);
	sm->setValue(1.0);
	sm->setRange(1.0, 3.0);
	sm->setSingleStep(std::pow(10, -8));

	sz = new QDoubleSpinBox();
	sz->setDecimals(8);
	sz->setValue(1.0);
	sz->setRange(1.0, 3.0);
	sz->setSingleStep(std::pow(10, -8));

	zd = new QDoubleSpinBox();
	zd->setDecimals(0);
	zd->setValue(1.0);
	zd->setRange(0, std::pow(2, 63));
	zd->setSingleStep(1);

	originLabel = new QLabel("Origin x/y");
	speedLabel = new QLabel("Move/Zoom speed");
	zoomLabel = new QLabel("Target zoom level");

	layout = new QVBoxLayout();
	layout->addWidget(originLabel);
	layout->addWidget(ox);
	layout->addWidget(oy);
	layout->addWidget(speedLabel);
	layout->addWidget(sm);
	layout->addWidget(sz);
	layout->addWidget(zoomLabel);
	layout->addWidget(zd);

	box = new QGroupBox();
	box->setAlignment(Qt::AlignCenter);
	box->setStyleSheet("border-color:black");
	box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	box->setLayout(layout);

	animLayout->addWidget(box);
}

Location::~Location()
{
}

void Location::init()
{
	destination.x = ox->value();
	destination.y = oy->value();
	speedMove = sm->value();
	speedZoom = sz->value();
	zoomDest = zd->value();
}
