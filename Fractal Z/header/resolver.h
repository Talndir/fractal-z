#pragma once

#include <QtGui/QOpenGLFunctions_4_4_Core>
#include <QtGUi/QVector2D>
#include <QtGUi/QVector3D>
#include <QtGUi/QVector4D>
#include "../header/vec2.h"

class Resolver : public QOpenGLFunctions_4_4_Core
{
public:
	Resolver();

	void setUniformValue(int location, float value);
	void setUniformValue(int location, int value);
	void setUniformValue(int location, QVector2D value);
	void setUniformValue(int location, QVector3D value);
	void setUniformValue(int location, QVector4D value);
	void setUniformValue(int location, vec2 value);
};