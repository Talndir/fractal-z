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

// Shader class
#include "../header/shader/shaderClass.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// Using OpenGL 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// Using OpenGL x.3
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

	// Set up OpenGL viewport
	glViewport(0, 0, windowWidth, windowHeight);

	// Create rectangle
	GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f,		// Top left
		1.0f, 1.0f, 0.0f,		// Top right
		-1.0f, -1.0f, 0.0f,		// Bottom Left
		1.0f, -1.0f, 0.0f		// Bottom right
	};

	GLuint indices[] = {
		0, 1, 3,
		0, 2, 3
	};

	/* Load shaders into shader program */
	Shader myShader;

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Link vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Fill mode for polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	std::cout << "Ratio: " << ratio << std::endl;
	glm::vec2 c = glm::vec2(0.0f, 0.0f);
	bool bChangeMode = true;
	bool bCannotChangeMode = false;

	// Main game loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Poll for events
		glfwPollEvents();

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

		// Draw to window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (bChangeMode && !bCannotChangeMode)
		{	
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
			}

			c = origin;
			origin = glm::vec2(0.0f, 0.0f);
			zoom = 1;

			bChangeMode = false;
		}

		if (keys[GLFW_KEY_M] || keys[GLFW_KEY_J] || keys[GLFW_KEY_N])
			bCannotChangeMode = true;
		else
		{
			bCannotChangeMode = false;
			bChangeMode = false;
		}

		glViewport(0, 0, windowWidth, windowHeight);

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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap backbuffer to front
		glfwSwapBuffers(mainWindow);

		// Clear colour and depth buffers
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	return 0;
}