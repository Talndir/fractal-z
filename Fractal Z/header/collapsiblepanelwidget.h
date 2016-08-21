#pragma once

#include <QtWidgets/QScrollArea>
#include <QtCore/QPropertyAnimation>

class CollapsiblePanelWidget : public QScrollArea
{
	Q_OBJECT

public:
	CollapsiblePanelWidget(QWidget* parent = 0);

	void config(QRect _openRect, QRect _closeRect);

	int duration = 1000;

public slots:
	void open();
	void close();
	void toggle();

private:
	QPropertyAnimation* openAnimation;
	QPropertyAnimation* closeAnimation;
	QRect* openRect;
	QRect* closeRect;

	bool isOpen = true;
};