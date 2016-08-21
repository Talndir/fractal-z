#pragma once

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtCore/QTimer>

#include "../header/external.h"
#include "../header/fractal.h"

#include <QtWidgets/QPushButton>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_4_Core
{
	Q_OBJECT

public:
	OpenGLWidget(QWidget* parent = 0);

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	void runCompute();

public slots:
	void drawFrame();

protected:
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private:
	GLuint vertexLocation;
	GLuint matrixLocation;

	std::vector<float> vertices;
	std::vector<int> indices;

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vvbo;
	QOpenGLBuffer m_ebo;

	QTimer* updateTimer;

	Fractal fractal;

	QPushButton* button;
};