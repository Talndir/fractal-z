#pragma once

#include <vector>

#include <QtWidgets/QComboBox>
#include <QtCore/QDir>
#include <QtCore/QFile>

class ShaderFinder : public QComboBox
{
	Q_OBJECT

public:
	ShaderFinder();
	~ShaderFinder();

signals:
	void useShader(QString intName, QString extName);

public slots:
	void useShader(int index);

private:
	void search();

	std::vector<std::pair<QString, QString>> shaders;
};