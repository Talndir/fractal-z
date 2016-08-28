#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>

#include "variabletyped.h"

typedef std::vector<std::shared_ptr<VariableAbstract>> shaderVariables;

class Fractal
{
public:
	Fractal();
	~Fractal();

	Fractal(QString _internalName, QString _externalName);

	template<typename T>
	void addComputeVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose);

	template<typename T>
	void addRenderVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose);

	template<typename T>
	void addComputeVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose, T* _pointer);

	template<typename T>
	void addRenderVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose, T* _pointer);

	template<typename T>
	void addVariable(QString intName, QString extName, std::vector<T>& v, bool exp, char mode);

	void copyRenderVariableToCompute();
	void copyComputeVariableToRender();

	void init();

	void beginCompute();
	void beginRender();

	void endCompute();
	void endRender();

	QOpenGLShaderProgram* computeProgram;
	QOpenGLShaderProgram* renderProgram;

	shaderVariables computeVariables;
	shaderVariables renderVariables;

private:
	QString internalName;
	QString externalName;
	QString root;

	QOpenGLShader* computeShader;
	QOpenGLShader* vertexShader;
	QOpenGLShader* fragmentShader;
};


template<typename T>
void Fractal::addComputeVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose)
{
	computeVariables.emplace_back(new VariableTyped<T>(_internalName, _externalName, _value, _value_default, _minimum_default, _maximum_defualt, _expose));
}

template<typename T>
void Fractal::addRenderVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose)
{
	renderVariables.emplace_back(new VariableTyped<T>(_internalName, _externalName, _value, _value_default, _minimum_default, _maximum_defualt, _expose));
}

template<typename T>
void Fractal::addComputeVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose, T * _pointer)
{
	computeVariables.emplace_back(new VariableTyped<T>(_internalName, _externalName, _value, _value_default, _minimum_default, _maximum_defualt, _expose, _pointer));
}

template<typename T>
void Fractal::addRenderVariable(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose, T * _pointer)
{
	renderVariables.emplace_back(new VariableTyped<T>(_internalName, _externalName, _value, _value_default, _minimum_default, _maximum_defualt, _expose, _pointer));
}

template<typename T>
inline void Fractal::addVariable(QString intName, QString extName, std::vector<T>& v, bool exp, char mode)
{
	switch (mode)
	{
	case 'r':
		addRenderVariable(intName, extName, v.at(0), v.at(1), v.at(2), v.at(3), exp);
		break;
	case 'c':
		addComputeVariable(intName, extName, v.at(0), v.at(1), v.at(2), v.at(3), exp);
		break;
	case 'R':
		addRenderVariable(intName, extName, v.at(0), v.at(1), v.at(2), v.at(3), exp);
		copyRenderVariableToCompute();
		break;
	case 'C':
		addComputeVariable(intName, extName, v.at(0), v.at(1), v.at(2), v.at(3), exp);
		copyComputeVariableToRender();
		break;
	}
}
