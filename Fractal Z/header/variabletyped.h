#pragma once

#include "../header/variableabstract.h"
#include "../header/external.h"

template<typename T>
class VariableTyped : public VariableAbstract
{
public:
	VariableTyped();
	~VariableTyped();

	VariableTyped(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_defualt, bool _expose);

	void useValue(QOpenGLShaderProgram* program) override;

	QString internalName;
	QString externalName;
	T value;
	T value_default;
	T minimum_default;
	T maximum_default;
	bool expose;
};

template<typename T>
inline void VariableTyped<T>::useValue(QOpenGLShaderProgram * program)
{
	// Program should be bound, and will not be released here
	program->setUniformValue(internalName.toUtf8().constData(), value);
}

template<typename T>
inline VariableTyped<T>::VariableTyped()
{
}

template<typename T>
inline VariableTyped<T>::~VariableTyped()
{
}

template<typename T>
inline VariableTyped<T>::VariableTyped(QString _internalName, QString _externalName, T _value, T _value_default, T _minimum_default, T _maximum_default, bool _expose)
{
	internalName = _internalName;
	externalName = _externalName;
	value = _value;
	value_default = _value_default;
	minimum_default = _minimum_default;
	maximum_default = _maximum_default;
	expose = _expose;
}
