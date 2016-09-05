#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

#include <QtWidgets/QApplication>

#include "../header/openglwidget.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
float RATIO = float(float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));
int BLOCK_WIDTH = 16;
int BLOCK_HEIGHT = 16;
int IMAGE_WIDTH = BLOCKS_TOTAL_HORIZONTAL * BLOCK_WIDTH;
int IMAGE_HEIGHT = BLOCKS_TOTAL_VERTICAL * BLOCK_HEIGHT;

CollapsiblePanelWidget* pane;
QVBoxLayout* paneLayout;
QGroupBox* paneBox;

CollapsiblePanelWidget* anim;
QVBoxLayout* animLayout;
QGroupBox* animBox;

Resolver* r;

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

	pane = new CollapsiblePanelWidget(&window);
	pane->setStyleSheet("background-color:white");
	pane->setWidgetResizable(true);
	pane->setGeometry(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT);
	pane->duration = 500;
	pane->config(QRect(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT), QRect(-WINDOW_WIDTH / 5, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT));

	paneBox = new QGroupBox();
	pane->setWidget(paneBox);

	paneLayout = new QVBoxLayout();
	paneLayout->setSpacing(10);
	paneLayout->setAlignment(Qt::AlignTop);

	pane->widget()->setLayout(paneLayout);
	pane->show();

	anim = new CollapsiblePanelWidget(&window);
	anim->setStyleSheet("background-color:white");
	anim->setWidgetResizable(true);
	anim->setGeometry(WINDOW_WIDTH * 0.8, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT);
	anim->duration = 500;
	anim->config(QRect(WINDOW_WIDTH * 0.8, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT), QRect(WINDOW_WIDTH, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT));

	animBox = new QGroupBox();
	anim->setWidget(animBox);

	animLayout = new QVBoxLayout();
	animLayout->setSpacing(10);
	animLayout->setAlignment(Qt::AlignTop);

	anim->widget()->setLayout(animLayout);
	anim->show();

	window.show();

	return app.exec();
}