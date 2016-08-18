#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#endif

#include <QtWidgets/QApplication>

#include "../header/openglwidget.h"

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

	window.show();

	return app.exec();
}