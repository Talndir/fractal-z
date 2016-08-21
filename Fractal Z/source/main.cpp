#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

#include <QtWidgets/QApplication>

#include "../header/openglwidget.h"

//QScrollArea* pane;
CollapsiblePanelWidget* pane;
QVBoxLayout* paneLayout;

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(4);
	format.setSamples(24);
	format.setVersion(4, 4);
	format.setRenderableType(QSurfaceFormat::OpenGL);

	OpenGLWidget window;
	window.setFormat(format);
	window.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	/*
	pane = new QScrollArea(&window);
	pane->setStyleSheet("background-color:white");
	pane->setWidgetResizable(true);
	pane->setGeometry(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT);
	*/

	pane = new CollapsiblePanelWidget(&window);
	pane->setStyleSheet("background-color:white");
	pane->setWidgetResizable(true);
	pane->setGeometry(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT);
	pane->duration = 500;
	pane->config(QRect(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT), QRect(-WINDOW_WIDTH / 5, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT));

	QGroupBox* box = new QGroupBox();
	pane->setWidget(box);

	paneLayout = new QVBoxLayout();
	paneLayout->setSpacing(10);
	paneLayout->setAlignment(Qt::AlignTop);

	pane->widget()->setLayout(paneLayout);
	pane->show();

	window.show();

	return app.exec();
}