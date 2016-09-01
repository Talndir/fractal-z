#include "../header/resolver.h"

Resolver::Resolver()
{
	initializeOpenGLFunctions();
}

void Resolver::setUniformValue(int location, float value)
{
	glUniform1f(location, value);
}

void Resolver::setUniformValue(int location, int value)
{
	glUniform1i(location, value);
}

void Resolver::setUniformValue(int location, QVector2D value)
{
	glUniform2f(location, value.x(), value.y());
}

void Resolver::setUniformValue(int location, QVector3D value)
{
	glUniform3f(location, value.x(), value.y(), value.z());
}

void Resolver::setUniformValue(int location, QVector4D value)
{
	glUniform4f(location, value.x(), value.y(), value.z(), value.w());
}