#include "../../header/shader/computeShader.h"

ComputeShader::ComputeShader()
{}

ComputeShader::ComputeShader(const GLchar* computeSourcePath)
{
	this->init(computeSourcePath);
}

void ComputeShader::init(const GLchar* computeSourcePath)
{
	std::string computeCode;
	std::ifstream cShaderFile;

	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		cShaderFile.open(computeSourcePath);
		std::stringstream cShaderStream;

		cShaderStream << cShaderFile.rdbuf();

		cShaderFile.close();

		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure error)
	{
		std::cout << "Error reading shader files for vertex and fragment shaders" << std::endl;
	}

	const GLchar* cShaderCode = computeCode.c_str();

	GLuint compute;
	GLint success;
	GLchar infolog[512];

	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);

	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(compute, 512, NULL, infolog);
		std::cout << "Error: Compute shader compilation failed" << std::endl;
		std::cout << infolog << std::endl;
	}

	this->program = glCreateProgram();
	glAttachShader(this->program, compute);
	glLinkProgram(this->program);

	glGetProgramiv(this->program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infolog);
		std::cout << "Error: Shader program linking failed" << std::endl;
		std::cout << infolog << std::endl;
	}

	glDeleteShader(compute);
}

void ComputeShader::use()
{
	glUseProgram(this->program);
}