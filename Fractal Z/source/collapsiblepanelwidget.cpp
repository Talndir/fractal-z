#include "../header/collapsiblepanelwidget.h"

CollapsiblePanelWidget::CollapsiblePanelWidget(QWidget* parent)
{
	setParent(parent);

	openAnimation = new QPropertyAnimation(this, "geometry");
	closeAnimation = new QPropertyAnimation(this, "geometry");
}

void CollapsiblePanelWidget::config(QRect _openRect, QRect _closeRect)
{
	if (isOpen)
		this->setGeometry(_openRect);
	else
		this->setGeometry(_closeRect);

	openRect = new QRect(_openRect);
	closeRect = new QRect(_closeRect);
	openAnimation->setEndValue(*openRect);
	closeAnimation->setEndValue(*closeRect);
}

void CollapsiblePanelWidget::open()
{
	closeAnimation->stop();

	openAnimation->setStartValue(this->geometry());
	float p = 1.f - float(float(this->geometry().x() + this->width()) / float(openRect->width()));
	openAnimation->setDuration(p * duration);

	openAnimation->start();
}

void CollapsiblePanelWidget::close()
{
	openAnimation->stop();

	closeAnimation->setStartValue(this->geometry());
	float p = float(float(this->geometry().x() + this->width()) / float(openRect->width()));
	closeAnimation->setDuration(p * duration);

	closeAnimation->start();
}

void CollapsiblePanelWidget::toggle()
{
	if (isOpen)
		close();
	else
		open();

	isOpen = !isOpen;
}