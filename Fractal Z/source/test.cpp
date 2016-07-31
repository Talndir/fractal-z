// ===============================================================
/* INCLUDES */
// GLEW	(Includes OpenGL too)
#define GLEW_STATIC		// Required for GLEW static compile
#include <GLEW\glew.h>	// Entire GLEW library

// GLFW (Must be included AFTER GLEW)
#include <GLFW\glfw3.h>	// Entire GLFW library

// SOIL for image loading
#include <SOIL\SOIL.h>	// Entire SOIL library

// GLM Mathematics
#include <GLM\glm.hpp>	// Main file for GLM
#include <GLM\gtc\type_ptr.hpp>

// GLFW callback functions
#include "../header/callback/keyCallback.h"

// Externs/globals
#include "../header/external.h"

// Shader classes
#include "../header/shader/shaderClass.h"
#include "../header/shader/computeShader.h"

// Other
#include <vector>
#include <iostream>

// ===============================================================
/* Global variables */
GLuint windowWidth = 1280;
GLuint windowHeight = 720;

std::vector<bool> keys(1024, false);
float ratio = (float)windowWidth / (float)windowHeight;
float zoom = 1.0f;
glm::vec2 origin = glm::vec2(0.0f, 0.0f);
int mode = 1;

// ===============================================================
/* Forward declarations */


// ===============================================================
/* Main */
int main()
{
	// Initialise and configure GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// Using OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);	// Using OpenGL x.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Using OpenGL core profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	// Do not allow user to resize window

												// Make a window
	GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Fractal Z", nullptr, nullptr);		// Create main window

	if (mainWindow == nullptr)
	{
		// Error here
		return -1;	// Return something
	}

	glfwMakeContextCurrent(mainWindow);		// Make mainWindow the main context on current thread

											// Bind callback functions for GLFW
	glfwSetKeyCallback(mainWindow, keyCallback);

	// Initialise GLEW
	glewExperimental = GL_TRUE;		// OpenGL core profile only

	if (glewInit() != GLEW_OK)
	{
		// Error here
		return -1;	// Return something
	}

	if (GLEW_ARB_gpu_shader_fp64)
		std::cout << "Extension GL_ARB_gpu_shader_fp64 enabled!" << std::endl;

	if (GLEW_ARB_compute_variable_group_size)
		std::cout << "Extension GL_ARB_compute_variable_group_size enabled!" << std::endl;

	// Set up OpenGL viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// Create rectangle
	/*
	GLfloat vertices[] = {
	-1.0f, 1.0f, 0.0f,		// Top left
	1.0f, 1.0f, 0.0f,		// Top right
	-1.0f, -1.0f, 0.0f,		// Bottom Left
	1.0f, -1.0f, 0.0f		// Bottom right
	};
	*/


	GLfloat vertices[19 * 19 * 3];		// 19 rows * 19 columns * 3 ordinates

	for (int i = -9; i < 10; ++i)
	{
		for (int j = -9; j < 10; ++j)
		{
			int index = (((i + 9) * 19) + j + 9) * 3;
			float x = float(j) / 8;
			float y = float(i) / 8;
			vertices[index + 0] = x;		// X co-ord
			vertices[index + 1] = y;		// Y co-ord
			vertices[index + 2] = 0.0f;		// Z co-ord (0 because on a plat plane)
											//std::cout << index << std::endl;
											//std::cout << vertices[index] << ", " << vertices[index + 1] << ", " << vertices[index + 2] << std::endl;
											//std::cin.ignore();
		}
	}

	/*
	GLuint indices[] = {
	0, 1, 3,
	0, 2, 3
	};
	*/

	/*
	GLuint indices[19 * 19 * 3 * 2];	// 19 rows * 19 columns * 3 vertices per triangle * 2 triangles per rectangle

	for (int i = 0; i < 18; ++i)
	{
	for (int j = 0; j < 18; ++j)
	{
	int index = ((i * 19) + j) * 6;
	indices[index + 0] = (i * 19) + j;			// Top left
	indices[index + 1] = (i * 19) + 1 + j;		// Top right
	indices[index + 2] = (i * 19) + 1 + 19 + j;	// Bottom right
	indices[index + 3] = (i * 19) + j;			// Top left
	indices[index + 4] = (i * 19) + 19 + j;		// Bottom left
	indices[index + 5] = (i * 19) + 19 + 1 + j;	// Bottom right
	//std::cout << index << std::endl;
	//std::cout << vertices[indices[index] * 3] << ", " << vertices[(indices[index] * 3) + 1] << std::endl;
	//std::cout << vertices[indices[index + 1] * 3] << ", " << vertices[(indices[index + 1] * 3) + 1] << std::endl;
	//std::cout << vertices[indices[index + 2] * 3] << ", " << vertices[(indices[index + 2] * 3) + 1] << std::endl;
	//std::cin.ignore();
	}
	}
	*/

	std::vector<int> indices;
	for (int i = 0; i < 18; ++i)
	{
		for (int j = 0; j < 18; ++j)
		{
			int index = ((i * 19) + j) * 6;
			indices.push_back((i * 19) + j);			// Top left
			indices.push_back((i * 19) + 1 + j);		// Top right
			indices.push_back((i * 19) + 1 + 19 + j);	// Bottom right
			indices.push_back((i * 19) + j);			// Top left
			indices.push_back((i * 19) + 19 + j);		// Bottom left
			indices.push_back((i * 19) + 19 + 1 + j);	// Bottom right
														//std::cout << index << std::endl;
														//std::cout << vertices[indices[index] * 3] << ", " << vertices[(indices[index] * 3) + 1] << std::endl;
														//std::cout << vertices[indices[index + 1] * 3] << ", " << vertices[(indices[index + 1] * 3) + 1] << std::endl;
														//std::cout << vertices[indices[index + 2] * 3] << ", " << vertices[(indices[index + 2] * 3) + 1] << std::endl;
														//std::cin.ignore();
		}
	}

	/*
	Triangles look like this:
	1-----2			1 = Top left
	|\	  |			2 = Top right
	|  \  |			3 = Bottom left
	|    \|			4 = Bottom right
	3-----4			1 and 4 are shared, 2 and 3 are not
	*/

	// SSBO for pixel data
	/*
	auto pixels = new GLuint[18][18][80][1];	// One element per pixel. Compute shader converts normalised iteration count into int

	for (int i = 0; i < 18; ++i)
	{
	for (int j = 0; j < 18; ++j)
	{
	for (int k = 0; k < 80; ++k)
	{
	for (int l = 0; l < 1; ++l)
	pixels[i][j][k][l] = 0;
	}
	}
	}
	*/

	std::vector<std::vector<std::vector<std::vector<GLint>>>> pixels;
	std::vector<std::vector<std::vector<GLint>>> temp3;
	std::vector<std::vector<GLint>> temp2;
	std::vector<GLint> temp1;

	for (int i = 0; i < 18; ++i)
		temp1.push_back(0);
	for (int i = 0; i < 18; ++i)
		temp2.push_back(temp1);

	for (int i = 0; i < temp2.size(); ++i)
	{
		for (int j = 0; j < temp2.at(i).size(); ++j)
		{
			temp2.at(i).at(j) = i + j;
			std::cout << i + j << " ";
		}

		std::cout << std::endl;
	}

	for (int i = 0; i < 80; ++i)
		temp3.push_back(temp2);
	for (int i = 0; i < 45; ++i)
		pixels.push_back(temp3);

	auto t = new GLint[18][18];

	for (int i = 0; i < 18; ++i)
	{
		for (int j = 0; j < 18; ++j)
		{
			t[i][j] = i + j;
			std::cout << t[i][j] << " ";
		}

		std::cout << std::endl;
	}

	auto tt = new GLint[18 * 18];

	for (int i = 0; i < 18; ++i)
	{
		for (int j = 0; j < 18; ++j)
		{
			tt[i * 18 + j] = i + j;
			std::cout << tt[i * 18 + j] << " ";
		}

		std::cout << std::endl;
	}

	/* Load shaders into shader program */
	Shader myShader;
	ComputeShader cShader;

	/* Create, set up and bind vertex array object */
	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create VBO of rectangle
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create EBO of rectangle
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	// Link vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Create SSBO of pixels
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, temp2.size() * temp1.size() * sizeof(int), temp2.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	memcpy(p, t, sizeof(t));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	// TEST UBO
	/*
	GLuint ubo;
	float test[] = { 1.0, 0.5, 0.5, 1.0 };

	GLuint blockIndex = glGetUniformBlockIndex(myShader.program, "abc");
	glUniformBlockBinding(myShader.program, blockIndex, 3);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 16, test, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, ubo);
	*/

	// Fill mode for polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec2 c = glm::vec2(0.0f, 0.0f);

	bool bChangeMode = true;
	bool bCannotChangeMode = false;

	//int vCount = 3;

	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "Error: " << err << std::endl;

	// Main game loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Poll for events
		glfwPollEvents();
		//std::cout << "1" << std::endl;
		// Check if ESC pressed, and if so alert window to close
		if (keys[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(mainWindow, GL_TRUE);

		if (keys[GLFW_KEY_LEFT])
			origin.x -= 0.04 / zoom;
		if (keys[GLFW_KEY_RIGHT])
			origin.x += 0.04 / zoom;
		if (keys[GLFW_KEY_UP])
			origin.y += 0.04 / zoom;
		if (keys[GLFW_KEY_DOWN])
			origin.y -= 0.04 / zoom;

		if (keys[GLFW_KEY_M])
		{
			mode = 1;
			bChangeMode = true;
		}
		if (keys[GLFW_KEY_J])
		{
			mode = 2;
			bChangeMode = true;
		}
		if (keys[GLFW_KEY_N])
		{
			mode = 3;
			bChangeMode = true;
		}
		if (keys[GLFW_KEY_C])
		{
			mode = 4;
			bChangeMode = true;
		}

		while ((err = glGetError()) != GL_NO_ERROR)
			std::cout << "Error: " << err << std::endl;

		// Draw to window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (bChangeMode && !bCannotChangeMode)
		{
			/*
			switch (mode)
			{
			case 1:
			myShader = Shader("shaders/mandelbrot_1/mandelbrot_1_vertex.glsl", "shaders/mandelbrot_1/mandelbrot_1_fragment.glsl");
			break;
			case 2:
			myShader = Shader("shaders/julia_1/julia_1_vertex.glsl", "shaders/julia_1/julia_1_fragment.glsl");
			break;
			case 3:
			myShader = Shader("shaders/newton_1/newton_1_vertex.glsl", "shaders/newton_1/newton_1_fragment.glsl");
			break;
			case 4:
			myShader = Shader("shaders/collatz_1/collatz_1_vertex.glsl", "shaders/collatz_1/collatz_1_fragment.glsl");
			break;
			}
			*/
			//std::cout << "2" << std::endl;
			switch (mode)
			{
			case 1:
				//cShader = ComputeShader("shaders/mandelbrot_2/mandelbrot_2_compute.glsl");
				myShader = Shader("shaders/mandelbrot_2/mandelbrot_2_vertex.glsl", "shaders/mandelbrot_2/mandelbrot_2_fragment.glsl");
				break;
			case 2:
				cShader = ComputeShader("shaders/julia_2/julia_2_compute.glsl");
				break;
			case 3:
				cShader = ComputeShader("shaders/newton_2/newton_2_compute.glsl");
				break;
			case 4:
				cShader = ComputeShader("shaders/collatz_2/collatz_2_compute.glsl");
				break;
			}


			c = origin;
			origin = glm::vec2(0.0f, 0.0f);
			//origin = glm::vec2(-1.1f, 0.30f);
			zoom = 1;
			//zoom = 16;

			bChangeMode = false;
		}
		//std::cout << "3" << std::endl;
		//myShader = Shader("shaders/mandelbrot_1/mandelbrot_1_vertex.glsl", "shaders/mandelbrot_1/mandelbrot_1_fragment.glsl");
		myShader = Shader("shaders/mandelbrot_2/mandelbrot_2_vertex.glsl", "shaders/mandelbrot_2/test.glsl");

		if (keys[GLFW_KEY_M] || keys[GLFW_KEY_J] || keys[GLFW_KEY_N] || keys[GLFW_KEY_C])
			bCannotChangeMode = true;
		else
		{
			bCannotChangeMode = false;
			bChangeMode = false;
		}

		glViewport(0, 0, windowWidth, windowHeight);
		//std::cout << "4" << std::endl;
		// Compute shader
		//cShader.use();
		//glDispatchComputeGroupSizeARB(18, 18, 1, 80, 45, 1);
		//glDispatchComputeGroupSizeARB(1, 1, 1, 1, 1, 1);
		//glDispatchCompute(1, 1, 1);
		//std::cout << "5" << std::endl;
		// Vertex/fragment shaders
		myShader.use();

		GLint ratioLoc = glGetUniformLocation(myShader.program, "ratio");
		glUniform1f(ratioLoc, ratio);

		GLint zoomLoc = glGetUniformLocation(myShader.program, "zoom");
		glUniform1f(zoomLoc, zoom);

		GLint originLoc = glGetUniformLocation(myShader.program, "origin");
		glUniform2f(originLoc, origin.x, origin.y);

		if (mode == 2)
		{
			GLint cLoc = glGetUniformLocation(myShader.program, "c");
			glUniform2f(cLoc, c.x, c.y);
		}

		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 19 * 19 * 3 * 2, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, vCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap backbuffer to front
		glfwSwapBuffers(mainWindow);

		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		if (vCount < 19 * 19 * 2 * 3)
		vCount += 3;

		std::cout << vCount << std::endl;
		std::cin.ignore();
		*/

		//std::cout << "DONE" << std::endl;
	}

	return 0;
}