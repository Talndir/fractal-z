#include "../header/openglwidget.h"

QVector2D renderOffset(0.0f, 0.0f);
QVector2D offset(0.01f, 0.01f);
QVector2D majorOffset(0.01f, 0.01f);
vec2 origin(0.0, 0.0);
float zoom = 1.f;
int maxIterations = 100;

bool autoIterations = false;

ERENDERMODE rendermodeLR = ALL;
ERENDERMODE rendermodeTB = NONE;

bool keys[1024];

bool fill = true;

vec2 start(0.0, 0.0);
vec2 end(-1.7690406658, 0.0054656756);
vec2 delta(end.x - start.x, end.y - start.y);
vec2 halfdDelta(delta.x / 2.0, delta.y / 2.0);
vec2 currentDelta(end.x - origin.x, end.y - origin.y);
vec2 d(0.0, 0.0);
int i = 0;
bool go = false;
double dp = 2.l / (WINDOW_HEIGHT * std::pow(2, 26));
double speedz = 1.003l;
double speedm = 1.01l;

OpenGLWidget::OpenGLWidget(QWidget * parent)
{
}

void OpenGLWidget::initializeGL()
{
	resolver = new Resolver();
	r = resolver;

	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	m_vao.create();
	QOpenGLBuffer m_vvbo(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer m_ebo(QOpenGLBuffer::IndexBuffer);

	// Fill mode for polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Shaders
	createFractal("mandelbrot", "Mandelbrot Set");

	resizeGL(WINDOW_WIDTH, WINDOW_HEIGHT);

	paneButton = new QPushButton(this);
	paneButton->setGeometry(QRect(0, 0, 25, 25));
	paneButton->setText("<->");
	connect(paneButton, SIGNAL(pressed()), pane, SLOT(toggle()));
	paneButton->show();

	animButton = new QPushButton(this);
	animButton->setGeometry(QRect(WINDOW_WIDTH - 25, 0, 25, 25));
	animButton->setText("<->");
	connect(animButton, SIGNAL(pressed()), ::anim, SLOT(toggle()));
	animButton->show();

	updateTimer = new QTimer(this);
	updateTimer->setInterval(1000.f / 1000.f);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(drawFrame()));
	updateTimer->start();

	animLayout->addWidget(anim.buttonAdd);
	animLayout->addWidget(anim.buttonDelete);

	for (unsigned int i = 0; i < 1024; ++i)
		keys[i] = false;

	/*
	Location loc;

	loc.destination = vec2(-1.7690406658, 0.0054656756);
	loc.speedMove = 1.045;
	loc.speedZoom = 1.003;
	loc.zoomDest = std::pow(2, 26);
	//anim.locations.push_back(loc);

	loc.destination = vec2(-1.7690406658, 0.0054656756);
	loc.speedMove = 1.0;
	loc.speedZoom = 1.013;
	loc.zoomDest = 1.0;
	//anim.locations.push_back(loc);

	loc.destination = vec2(0.0, 0.0);
	loc.speedMove = 1.025;
	loc.speedZoom = 1.0;
	loc.zoomDest = 1.0;
	//anim.locations.push_back(loc);

	loc.destination = vec2(0.2639296294227568, -0.0026976800505326);
	loc.speedMove = 1.04;
	loc.speedZoom = 1.013;
	loc.zoomDest = std::pow(2, 43);
	anim.locations.push_back(loc);

	loc.destination = vec2(0.2639296294227568, -0.0026976800505326);
	loc.speedMove = 1.0;
	loc.speedZoom = 1.013;
	loc.zoomDest = 1.0;
	anim.locations.push_back(loc);

	loc.destination = vec2(0.0, 0.0);
	loc.speedMove = 1.025;
	loc.speedZoom = 1.0;
	loc.zoomDest = 1.0;
	anim.locations.push_back(loc);
	*/

	anim.origin = &origin;
	anim.zoom = &zoom;

	//showFullScreen();
	drawFrame();
}

void OpenGLWidget::paintGL()
{
	const qreal retinaScale = devicePixelRatio();
	glClear(GL_COLOR_BUFFER_BIT);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	majorOffset += offset;
	fractal.beginRender();

	if (fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_vao.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	m_vao.release();

	fractal.endRender();
	majorOffset -= offset;

	if (go)
	{
		go = anim.nextFrame();
		fractal.computeVariables.at(0)->setValue();
		fractal.computeVariables.at(fractal.computeVariables.size() - 1)->setValue();
		rendermodeLR = ALL;
	}
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() < 1024)
		keys[event->key()] = true;

	if (event->key() == Qt::Key::Key_Q)
	{
		zoom *= 2.0f;
		fractal.computeVariables.at(fractal.computeVariables.size() - 1)->setValue();
		rendermodeLR = ALL;
	}
	else if (event->key() == Qt::Key::Key_E)
	{
		zoom /= 2.0f;
		fractal.computeVariables.at(fractal.computeVariables.size() - 1)->setValue();
		rendermodeLR = ALL;
	}
	else if (event->key() == Qt::Key::Key_P)
	{
		QString file = QFileDialog::getSaveFileName(this, "Save as...", "name", "PNG (*.png);; BMP (*.bmp);;TIFF (*.tiff *.tif);; JPEG (*.jpg *.jpeg)");
		this->grabFramebuffer().save(file);
	}
	else if (event->key() == Qt::Key::Key_F11)
	{
		if (this->windowState() == Qt::WindowState::WindowFullScreen)
			showNormal();
		else
			showFullScreen();
	}
	else if (event->key() == Qt::Key::Key_0)
		go = !go;
	else if (event->key() == Qt::Key::Key_9)
	{
		go = false;
		anim.index = -1;
		anim.next = true;
	}
	else if (event->key() == Qt::Key::Key_I)
		autoIterations = !autoIterations;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() < 1024)
		keys[event->key()] = false;
}

void OpenGLWidget::drawFrame()
{
	getKeys();
	runCompute();
	update();
}

void OpenGLWidget::runCompute()
{
	if (rendermodeLR || rendermodeTB)
	{
		makeCurrent();

		if (autoIterations)
		{
			maxIterations = std::max(100 + log2f(zoom) * 20, 10.f);
			fractal.computeVariables.at(1)->setValue();
		}

		switch (rendermodeLR)
		{
		case NONE:
			break;
		case ALL:
			renderOffset = QVector2D(0.0f, 0.0f);
			fractal.beginCompute();
			glDispatchCompute(BLOCKS_TOTAL_HORIZONTAL, BLOCKS_TOTAL_VERTICAL, 1);
			break;
		case LEFT:
			renderOffset = QVector2D(0.0f, 0.0f);
			fractal.beginCompute();
			glDispatchCompute(1, BLOCKS_TOTAL_VERTICAL, 1);
			break;
		case RIGHT:
			renderOffset = QVector2D(IMAGE_WIDTH - BLOCK_WIDTH, 0.0f);
			fractal.beginCompute();
			glDispatchCompute(1, BLOCKS_TOTAL_VERTICAL, 1);
			break;
		}

		switch (rendermodeTB)
		{
		case NONE:
			break;
		case TOP:
			renderOffset = QVector2D(0.0f, IMAGE_HEIGHT - BLOCK_HEIGHT);
			fractal.beginCompute();
			glDispatchCompute(BLOCKS_TOTAL_HORIZONTAL, 1, 1);
			break;
		case BOTTOM:
			renderOffset = QVector2D(0.0f, 0.0f);
			fractal.beginCompute();
			glDispatchCompute(BLOCKS_TOTAL_HORIZONTAL, 1, 1);
			break;
		}

		rendermodeLR = NONE;
		rendermodeTB = NONE;

		fractal.endCompute();
		doneCurrent();
	}
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
	fractal.addComputeVariable("offset", "", majorOffset, majorOffset, majorOffset, majorOffset, false, &majorOffset);
	fractal.copyComputeVariableToRender();
	//fractal.addComputeVariable("origin", "Origin", origin, origin, vec2(-4.f, -4.f), vec2(4.f, 4.f), true, &origin);
	fractal.addComputeVariable("ratio", "", RATIO, RATIO, RATIO, RATIO, false, &RATIO);
	fractal.addComputeVariable("zoom", "Zoom", zoom, zoom, 0.0f, float(std::pow(2, 64)), true, &zoom);

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

void OpenGLWidget::getKeys()
{
	if (rendermodeLR != ALL)
	{
		if (keys[Qt::Key::Key_W])
		{
			origin.y += (ORIGIN_MOVE / zoom);
			offset.setY(offset.y() + PIXEL_MOVE);
			fractal.computeVariables.at(0)->setValue();
		}
		if (keys[Qt::Key::Key_S])
		{
			origin.y -= (ORIGIN_MOVE / zoom);
			offset.setY(offset.y() - PIXEL_MOVE);
			fractal.computeVariables.at(0)->setValue();
		}
		if (keys[Qt::Key::Key_A])
		{
			origin.x -= (ORIGIN_MOVE / zoom);
			offset.setX(offset.x() - PIXEL_MOVE);
			fractal.computeVariables.at(0)->setValue();
		}
		if (keys[Qt::Key::Key_D])
		{
			origin.x += (ORIGIN_MOVE / zoom);
			offset.setX(offset.x() + PIXEL_MOVE);
			fractal.computeVariables.at(0)->setValue();
		}
	}

	if (keys[Qt::Key::Key_L])
		fill = false;
	if (keys[Qt::Key::Key_F])
		fill = true;

	if (offset.x() >= BLOCK_WIDTH)
	{
		majorOffset.setX(majorOffset.x() + BLOCK_WIDTH);
		rendermodeLR = RIGHT;
		offset.setX(offset.x() - BLOCK_WIDTH);
	}
	else if (offset.x() < 0)
	{
		majorOffset.setX(majorOffset.x() - BLOCK_WIDTH);
		rendermodeLR = LEFT;
		offset.setX(offset.x() + BLOCK_WIDTH);
	}
	if (offset.y() >= BLOCK_HEIGHT)
	{
		majorOffset.setY(majorOffset.y() + BLOCK_HEIGHT);
		rendermodeTB = TOP;
		offset.setY(offset.y() - BLOCK_HEIGHT);
	}
	else if (offset.y() < 0)
	{
		majorOffset.setY(majorOffset.y() - BLOCK_HEIGHT);
		rendermodeTB = BOTTOM;
		offset.setY(offset.y() + BLOCK_HEIGHT);
	}

	if (keys[Qt::Key::Key_R])
	{
		origin = vec2(0.0, 0.0);
		fractal.computeVariables.at(0)->setValue();
		zoom = 1.f;
		fractal.computeVariables.at(fractal.computeVariables.size() - 1)->setValue();
		rendermodeLR = ALL;
	}
	if (keys[Qt::Key::Key_C])
		rendermodeLR = ALL;

	if (majorOffset.x() >= IMAGE_WIDTH)
		majorOffset.setX(majorOffset.x() - IMAGE_WIDTH);
	else if (majorOffset.x() < 0)
		majorOffset.setX(majorOffset.x() + IMAGE_WIDTH);
	if (majorOffset.y() >= IMAGE_HEIGHT)
		majorOffset.setY(majorOffset.y() - IMAGE_HEIGHT);
	else if (majorOffset.y() < 0)
		majorOffset.setY(majorOffset.y() + IMAGE_HEIGHT);
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

	rendermodeLR = ALL;
}