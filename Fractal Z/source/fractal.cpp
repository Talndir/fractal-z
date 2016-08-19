#include "../header/fractal.h"

QString fractalRoot = "shaders/";

Fractal::Fractal()
{
	internalName = QString("test");
	externalName = QString("Test");
	root = fractalRoot + internalName + "/";
}

Fractal::~Fractal()
{
}

Fractal::Fractal(QString _internalName, QString _externalName)
{
	internalName = _internalName;
	externalName = _externalName;
	root = fractalRoot + internalName + "/";
}

void Fractal::copyRenderVariableToCompute()
{
	computeVariables.push_back(renderVariables.back());
}

void Fractal::copyComputeVariableToRender()
{
	renderVariables.push_back(computeVariables.back());
}

void Fractal::init()
{
	computeShader = new QOpenGLShader(QOpenGLShader::Compute);
	computeShader->compileSourceFile(root + internalName + "_compute.glsl");

	vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	vertexShader->compileSourceFile(root + internalName + "_vertex.glsl");

	fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	fragmentShader->compileSourceFile(root + internalName + "_fragment.glsl");

	computeProgram = new QOpenGLShaderProgram();
	computeProgram->addShader(computeShader);
	computeProgram->link();

	renderProgram = new QOpenGLShaderProgram();
	renderProgram->addShader(vertexShader);
	renderProgram->addShader(fragmentShader);
	renderProgram->link();
}

void Fractal::beginCompute()
{
	computeProgram->bind();

	for (unsigned int i = 0; i < computeVariables.size(); ++i)
		computeVariables.at(i)->useValue(computeProgram);
}

void Fractal::beginRender()
{
	renderProgram->bind();

	for (unsigned int i = 0; i < renderVariables.size(); ++i)
		renderVariables.at(i)->useValue(renderProgram);
}

void Fractal::endCompute()
{
	computeProgram->release();
}

void Fractal::endRender()
{
	renderProgram->release();
}