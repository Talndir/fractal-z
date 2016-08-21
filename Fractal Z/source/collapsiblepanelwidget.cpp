#include "../header/collapsiblepanelwidget.h"

CollapsiblePanelWidget::CollapsiblePanelWidget(QWidget* parent)
{
	setParent(parent);

	openAnimation = new QPropertyAnimation(this, "geometry");
	closeAnimation = new QPropertyAnimation(this, "geometry");
}

void CollapsiblePanelWidget::config(QRect _openRect, QRect _closeRect)
{
	this->setGeometry(_openRect);
	openRect = new QRect(_openRect);
	closeRect = new QRect(_closeRect);
	openAnimation->setEndValue(*closeRect);
	closeAnimation->setEndValue(*openRect);
}

void CollapsiblePanelWidget::open()
{
	closeAnimation->stop();

	openAnimation->setStartValue(this->geometry());
	float p = float((this->geometry().x() + this->width()) / openRect->width());
	openAnimation->setDuration(p * duration);

	openAnimation->start();
}

void CollapsiblePanelWidget::close()
{
	openAnimation->stop();

	closeAnimation->setStartValue(this->geometry());
	float p = 1.f - float((this->geometry().x() + this->width()) / openRect->width());
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