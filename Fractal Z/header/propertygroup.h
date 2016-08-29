#pragma once

#include <vector>
#include <memory>

#include <QtGui/QOpenGLShaderProgram>
#include <QtWidgets/QGroupBox>

#include "../header/property.h"
#include "../header/external.h"

class PropertyGroup
{
public:
	PropertyGroup(float min, float max, float t, QString name);
	PropertyGroup(int min, int max, int t, QString name);
	PropertyGroup(QVector2D min, QVector2D max, QVector2D t, QString name);
	PropertyGroup(QVector3D min, QVector3D max, QVector3D t, QString name);
	PropertyGroup(QVector4D min, QVector4D max, QVector4D t, QString name);

	~PropertyGroup();

	void createBox(QString name);
	void config();

	void getValue(float& v);
	void getValue(int& v);
	void getValue(QVector2D& v);
	void getValue(QVector3D& v);
	void getValue(QVector4D& v);

	void setValue(float v);
	void setValue(int v);
	void setValue(QVector2D v);
	void setValue(QVector3D v);
	void setValue(QVector4D v);

	std::vector<std::shared_ptr<Property>> properties;
	QGroupBox* box;
	QVBoxLayout* boxLayout;
};