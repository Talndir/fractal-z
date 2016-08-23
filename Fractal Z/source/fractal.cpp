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

	std::fstream file(QString(root + internalName + "_variables.txt").toUtf8().constData());
	std::string line = "";
	std::vector<std::string> data;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string item;

		while (std::getline(ss, item, ','))
			data.push_back(item);

		data.push_back(item);


		std::string type = data.at(0);
		std::string intName = data.at(1);
		std::string extName = data.at(2);
		bool exp = (data.at(7) == "true" ? true : false);
		char mode = data.at(8).at(0);

		if (type == "float")
		{
			std::vector<float> v;
			for (int i = 3; i < 7; ++i)
				v.push_back(std::atof(data.at(i).c_str()));

			addVariable(intName.c_str(), extName.c_str(), v, exp, mode);
		}
		else if (type == "int")
		{
			std::vector<int> v;
			for (int i = 3; i < 7; ++i)
				v.push_back(std::atoi(data.at(i).c_str()));

			addVariable(intName.c_str(), extName.c_str(), v, exp, mode);
		}
		else if (type == "vec2")
		{
			std::vector<QVector2D> v;
			for (int i = 3; i < 7; ++i)
			{
				std::stringstream ss2(data.at(i));
				std::vector<float> v2;
				std::string item2;

				while (std::getline(ss2, item2, ':'))
					v2.push_back(std::atof(item2.c_str()));

				v.push_back(QVector2D(v2.at(0), v2.at(1)));
			}

			addVariable(intName.c_str(), extName.c_str(), v, exp, mode);
		}
		else if (type == "vec3")
		{
			std::vector<QVector3D> v;
			for (int i = 3; i < 7; ++i)
			{
				std::stringstream ss2(data.at(i));
				std::vector<float> v2;
				std::string item2;

				while (std::getline(ss2, item2, ':'))
					v2.push_back(std::atof(item2.c_str()));

				v.push_back(QVector3D(v2.at(0), v2.at(1), v2.at(2)));
			}

			addVariable(intName.c_str(), extName.c_str(), v, exp, mode);
		}
		else if (type == "vec4")
		{
			std::vector<QVector4D> v;
			for (int i = 3; i < 7; ++i)
			{
				std::stringstream ss2(data.at(i));
				std::vector<float> v2;
				std::string item2;

				while (std::getline(ss2, item2, ':'))
					v2.push_back(std::atof(item2.c_str()));

				v.push_back(QVector4D(v2.at(0), v2.at(1), v2.at(2), v2.at(3)));
			}

			addVariable(intName.c_str(), extName.c_str(), v, exp, mode);
		}


		data.clear();
	}

	for (int i = 0; i < data.size(); ++i)
		QTextStream(stdout) << data.at(i).c_str() << endl;
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