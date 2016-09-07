#include "../header/shaderfinder.h"
#include <QtCore/QTextStream>

ShaderFinder::ShaderFinder()
{
	search();
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(useShader(int)));
}

ShaderFinder::~ShaderFinder()
{
}

void ShaderFinder::search()
{
	shaders.clear();
	this->clear();

	QDir dir("shaders/");
	QStringList list = dir.entryList();
	QFile info;

	for (unsigned int i = 0; i < list.size(); ++i)
	{
		info.setFileName("shaders/" + list.at(i) + "/" + list.at(i) + "_info.txt");
		QTextStream(stdout) << info.fileName() << endl;
		if (info.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			shaders.push_back(std::make_pair(list.at(i), info.readLine()));
			info.close();
			addItem(shaders.back().second);
		}
	}

	highlighted("Mandelbrot");
}

void ShaderFinder::useShader(int index)
{
	useShader(shaders.at(index).first, shaders.at(index).second);
}