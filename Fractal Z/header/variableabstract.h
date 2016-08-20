#pragma once

#include <QtGui/QOpenGLShaderProgram>

class VariableAbstract
{
public:
	VariableAbstract();
	~VariableAbstract();

	virtual void useValue(QOpenGLShaderProgram* program) = 0;
	virtual void createPropertyGroup() = 0;
};