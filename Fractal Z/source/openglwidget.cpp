#include "../header/openglwidget.h"

QVector2D renderOffset(0.0f, 0.0f);
QVector2D offset(0.0f, 0.0f);
QVector2D origin(0.0f, 0.0f);
float zoom = 1.f;

OpenGLWidget::OpenGLWidget(QWidget * parent)
{
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	m_vao.create();
	QOpenGLBuffer m_vvbo(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer m_ebo(QOpenGLBuffer::IndexBuffer);

	// Fill mode for polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Shaders
	createFractal("mandelbrot", "Mandelbrot Set");

	resizeGL(WINDOW_WIDTH, WINDOW_HEIGHT);

	button = new QPushButton(this);
	button->setGeometry(QRect(0, 0, 25, 25));
	button->setText("<->");
	connect(button, SIGNAL(pressed()), pane, SLOT(toggle()));
	button->show();

	updateTimer = new QTimer(this);
	updateTimer->setInterval(1000.f / 30.f);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(drawFrame()));
	updateTimer->start();

	update();
}

void OpenGLWidget::paintGL()
{
	const qreal retinaScale = devicePixelRatio();
	glClear(GL_COLOR_BUFFER_BIT);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	fractal.beginRender();

	m_vao.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	m_vao.release();

	fractal.endRender();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
}

void OpenGLWidget::drawFrame()
{
	runCompute();
	update();
}

void OpenGLWidget::runCompute()
{
	makeCurrent();

	fractal.beginCompute();

	glDispatchCompute(BLOCKS_TOTAL_HORIZONTAL, BLOCKS_TOTAL_VERTICAL, 1);

	fractal.endCompute();

	doneCurrent();
}

void OpenGLWidget::createFractal(QString intName, QString extName)
{
	delete paneLayout;
	paneLayout = new QVBoxLayout();
	paneLayout->setSpacing(10);
	paneLayout->setAlignment(Qt::AlignTop);

	delete paneBox;
	paneBox = new QGroupBox();
	pane->setWidget(paneBox);

	pane->widget()->setLayout(paneLayout);
	pane->show();

	fractal = Fractal(intName, extName);
	
	fractal.init();

	fractal.addComputeVariable("WINDOW_WIDTH", "", WINDOW_WIDTH, WINDOW_WIDTH, WINDOW_WIDTH, WINDOW_WIDTH, false, &WINDOW_WIDTH);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("WINDOW_HEIGHT", "", WINDOW_HEIGHT, WINDOW_HEIGHT, WINDOW_HEIGHT, WINDOW_HEIGHT, false, &WINDOW_HEIGHT);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("BLOCK_WIDTH", "", BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH, false, &BLOCK_WIDTH);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("BLOCK_HEIGHT", "", BLOCK_HEIGHT, BLOCK_HEIGHT, BLOCK_HEIGHT, BLOCK_HEIGHT, false, &BLOCK_HEIGHT);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("IMAGE_WIDTH", "", IMAGE_WIDTH, IMAGE_WIDTH, IMAGE_WIDTH, IMAGE_WIDTH, false, &IMAGE_WIDTH);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("IMAGE_HEIGHT", "", IMAGE_HEIGHT, IMAGE_HEIGHT, IMAGE_HEIGHT, IMAGE_HEIGHT, false, &IMAGE_HEIGHT);
	fractal.copyComputeVariableToRender();

	fractal.addComputeVariable("renderOffset", "", renderOffset, renderOffset, renderOffset, renderOffset, false, &renderOffset);
	fractal.addComputeVariable("offset", "", offset, offset, offset, offset, false, &offset);
	fractal.copyComputeVariableToRender();
	fractal.addComputeVariable("origin", "Origin", origin, origin, QVector2D(-2.f, -2.f), QVector2D(2.f, 2.f), true, &origin);
	fractal.addComputeVariable("ratio", "", RATIO, RATIO, RATIO, RATIO, false, &RATIO);
	fractal.addComputeVariable("zoom", "Zoom", zoom, zoom, 0.0f, 10.0f, true, &zoom);

	m_vao.bind();

	m_vvbo.create();
	m_vvbo.bind();
	m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vvbo.allocate(vertices.data(), vertices.size() * sizeof(float));

	m_ebo.create();
	m_ebo.bind();
	m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_ebo.allocate(indices.data(), indices.size() * sizeof(int));

	m_vao.release();

	m_vvbo.release();
	m_ebo.release();

	fractal.computeProgram->bind();
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	fractal.computeProgram->release();

	fractal.renderProgram->bind();
	m_vao.bind();
	m_vvbo.bind();
	fractal.renderProgram->enableAttributeArray("position");
	fractal.renderProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);
	m_vvbo.release();
	m_vao.release();
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	fractal.renderProgram->release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	RATIO = float(float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));
	IMAGE_WIDTH = BLOCKS_TOTAL_HORIZONTAL * BLOCK_WIDTH;
	IMAGE_HEIGHT = BLOCKS_TOTAL_VERTICAL * BLOCK_HEIGHT;

	pane->config(QRect(0, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT), QRect(-WINDOW_WIDTH / 5, 0, WINDOW_WIDTH / 5, WINDOW_HEIGHT));

	// Create block vertices
	{
		int a = BLOCKS_TOTAL_VERTICAL / 2;
		int b = (BLOCKS_TOTAL_VERTICAL + 1) - a;
		int m = BLOCKS_TOTAL_HORIZONTAL / 2;
		int n = (BLOCKS_TOTAL_HORIZONTAL + 1) - m;

		vertices.clear();

		// Tesselate render area with triangles
		for (int i = -a; i < b; ++i)
		{
			for (int j = -m; j < n; ++j)
			{
				float x = float(j) / (BLOCKS_HORIZONTAL / 2);
				float y = float(i) / (BLOCKS_VERTICAL / 2);
				vertices.push_back(x);		// X co-ord
				vertices.push_back(y);		// Y co-ord
				vertices.push_back(0.0f);	// Z co-ord (0 because on a flat plane)
			}
		}
	}

	// Create block indices (works with vertex co-ords for more efficient rendering)
	{
		int a = BLOCKS_TOTAL_HORIZONTAL + 1;

		indices.clear();

		// Loop through each triangle pair (square) getting their vertices in order
		for (int i = 0; i < BLOCKS_TOTAL_VERTICAL; ++i)
		{
			for (int j = 0; j < BLOCKS_TOTAL_HORIZONTAL; ++j)
			{
				int index = ((i * a) + j) * 6;
				indices.push_back((i * a) + j);			// Top left
				indices.push_back((i * a) + 1 + j);		// Top right
				indices.push_back((i * a) + 1 + a + j);	// Bottom right
				indices.push_back((i * a) + j);			// Top left
				indices.push_back((i * a) + a + j);		// Bottom left
				indices.push_back((i * a) + a + 1 + j);	// Bottom right
			}
		}
	}

	m_vao.bind();

	QOpenGLBuffer m_vvbo(QOpenGLBuffer::VertexBuffer);
	m_vvbo.create();
	m_vvbo.bind();
	m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vvbo.allocate(vertices.data(), vertices.size() * sizeof(float));
	

	QOpenGLBuffer m_ebo(QOpenGLBuffer::IndexBuffer);
	m_ebo.create();
	m_ebo.bind();
	m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_ebo.allocate(indices.data(), indices.size() * sizeof(int));

	m_vao.release();
	m_vvbo.release();
	m_ebo.release();

	glDeleteTextures(1, tex);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, IMAGE_WIDTH, IMAGE_HEIGHT);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_RED, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	fractal.computeProgram->bind();
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	fractal.computeProgram->release();

	fractal.renderProgram->bind();
	m_vao.bind();
	m_vvbo.bind();
	fractal.renderProgram->enableAttributeArray("position");
	fractal.renderProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);
	m_vvbo.release();
	m_vao.release();
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	fractal.renderProgram->release();
}