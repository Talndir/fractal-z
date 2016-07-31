#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW\glew.h>

class ComputeShader
{
public:
	GLuint program;

	ComputeShader(const GLchar* computeSourcePath);
	ComputeShader();

	void init(const GLchar* computeSourcePath);

	void use();
};