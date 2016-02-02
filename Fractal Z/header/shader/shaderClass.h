#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW\glew.h>

class Shader
{
public:
	GLuint program;

	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
	Shader();

	void init(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);

	void use();
};

#endif