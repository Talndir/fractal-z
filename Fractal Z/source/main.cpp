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
std::vector<bool> keys(1024, false);				// Which keys are pressed
float zoom = 1.0f;									// Zoom factor
glm::vec2 origin = glm::vec2(0.0f, 0.0f);			// Centre of screen in fractal co-ordinates
int maxIterations = 500;							// Maximum iterations
int mode = 1;										// Mandelbrot/Julia/Newton/Collatz
glm::ivec2 offset = glm::ivec2(0, 0);				// Pixels moved in a block from the (pixel) origin
glm::ivec2 majorOffset = glm::ivec2(0, 0);			// Blocks moved from the (pixel) origin
ERENDERMODE rendermodeLR = ALL;						// Rendermode for left/right, render all
ERENDERMODE rendermodeTB = NONE;					// Rendermode for top/bottom, render none (l/r is already rendering everything)
bool bRenderAllNow = true;							// Render entire screen immediately
float normaliser = 3200.f / float(maxIterations);	// Scale float to integer
bool bChangeColour = false;							// Whether or not to auto-scroll colours
float colourOffset = 0.0f;							// Colour offset for colouring
float colourScale = 1.0f;							// Scale hues up/down

/* DEFINES */
#define WINDOW_WIDTH 1920															// Window width in pixels
#define WINDOW_HEIGHT 1080															// Window height in pixels
#define BLOCK_WIDTH 16																// Width of each block
#define BLOCK_HEIGHT 16																// Height of each block
#define BLOCKS_HORIZONTAL (WINDOW_WIDTH / BLOCK_WIDTH)								// Number of blocks horizontally
#define BLOCKS_VERTICAL (WINDOW_HEIGHT / BLOCK_HEIGHT)								// Number of blocks vertically
#define BLOCKS_EXTRA_HORIZONTAL 1													// Extra horizontal buffer blocks
#define BLOCKS_EXTRA_VERTICAL 1														// Extra vertical buffer blocks
#define BLOCKS_TOTAL_HORIZONTAL (BLOCKS_HORIZONTAL + (2 * BLOCKS_EXTRA_HORIZONTAL))	// Total horizontal blocks
#define BLOCKS_TOTAL_VERTICAL (BLOCKS_VERTICAL + (2 * BLOCKS_EXTRA_VERTICAL))		// Total vertical blocks
#define IMAGE_WIDTH (BLOCKS_TOTAL_HORIZONTAL * BLOCK_WIDTH)							// Width of entire render area
#define IMAGE_HEIGHT (BLOCKS_TOTAL_VERTICAL * BLOCK_HEIGHT)							// Height of entire render area
#define PIXEL_MOVE 16																// Pixels moved per 'step'
#define ORIGIN_MOVE ((float(PIXEL_MOVE) / float(WINDOW_HEIGHT)) * 4)				// Origin movement per 'step'

GLuint windowWidth = WINDOW_WIDTH;													// Width of the render window
GLuint windowHeight = WINDOW_HEIGHT;												// Height of the render window
float ratio = (float)windowWidth / (float)windowHeight;								// Aspect ratio of the render window


// ===============================================================
/* Forward declarations */


// ===============================================================
/* Main */
int main()
{
	// Initialise and configure GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// Using OpenGL 4.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);	// Using OpenGL x.4
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
	
	// Create block vertices
	GLfloat vertices[(BLOCKS_TOTAL_HORIZONTAL + 1) * (BLOCKS_TOTAL_VERTICAL + 1) * 3];		// 19 rows * 19 columns * 3 ordinates

	{
		int a = BLOCKS_TOTAL_VERTICAL / 2;
		int b = (BLOCKS_TOTAL_VERTICAL + 1) - a;
		int m = BLOCKS_TOTAL_HORIZONTAL / 2;
		int n = (BLOCKS_TOTAL_HORIZONTAL + 1) - m;

		//std::cout << a << ", " << b << ", " << m << ", " << n << std::endl;

		// Tesselate render area with triangles
		for (int i = -a; i < b; ++i)
		{
			for (int j = -m; j < n; ++j)
			{
				int index = (((i + a) * (BLOCKS_TOTAL_HORIZONTAL + 1)) + j + m) * 3;	// Index of vertex array
				float x = float(j) / (BLOCKS_HORIZONTAL / 2);
				float y = float(i) / (BLOCKS_VERTICAL / 2);
				vertices[index + 0] = x ;		// X co-ord
				vertices[index + 1] = y ;		// Y co-ord
				vertices[index + 2] = 0.0f;		// Z co-ord (0 because on a flat plane)
				//std::cout << index << std::endl;
				//std::cout << vertices[index] << ", " << vertices[index + 1] << ", " << vertices[index + 2] << std::endl;
				//std::cin.ignore();
			}
		}
	}

	// Create block indices (works with vertex co-ords for more efficient rendering)
	std::vector<int> indices;
	{
		int a = BLOCKS_TOTAL_HORIZONTAL + 1;

		// Loop through each triangle pair (square) getting their vertices in order
		for (int i = 0; i < BLOCKS_TOTAL_VERTICAL; ++i)
		{
			for (int j = 0; j < BLOCKS_TOTAL_HORIZONTAL; ++j)
			{
				int index = ((i * a) + j) * 6;
				indices.push_back((i * a) + j);			// Top left
				indices.push_back((i * a) + 1 + j);		// Top right
				indices.push_back((i * a) + 1 + a + j);	// Bottom right
				indices.push_back((i * a) + j);			// Top left
				indices.push_back((i * a) + a + j);		// Bottom left
				indices.push_back((i * a) + a + 1 + j);	// Bottom right
				//std::cout << index << std::endl;
				//std::cout << vertices[indices[index] * 3] << ", " << vertices[(indices[index] * 3) + 1] << std::endl;
				//std::cout << vertices[indices[index + 1] * 3] << ", " << vertices[(indices[index + 1] * 3) + 1] << std::endl;
				//std::cout << vertices[indices[index + 2] * 3] << ", " << vertices[(indices[index + 2] * 3) + 1] << std::endl;
				//std::cin.ignore();
			}
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
	
	/*
	// Create image (for load/store)
	int* data = new int[IMAGE_WIDTH * IMAGE_HEIGHT];
	for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i)
		data[i] = 0;

	GLuint tex[1];
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16I, IMAGE_WIDTH, IMAGE_HEIGHT);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_RED_INTEGER, GL_INT, data);
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R16I);

	delete[] data;
	*/

	// Create image (for load/store)
	float* data = new float[IMAGE_WIDTH * IMAGE_HEIGHT];
	for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i)
		data[i] = 0.f;

	GLuint tex[1];
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, IMAGE_WIDTH, IMAGE_HEIGHT);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, GL_RED, GL_FLOAT, data);
	glBindImageTexture(0, tex[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

	
	// Fill mode for polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec2 c = glm::vec2(0.0f, 0.0f);	// Value of 'c' for Julia set

	bool bChangeMode = true;
	bool bCannotChangeMode = false;

	int vCount = indices.size();		// For vertex testing later, change to 0 to initiate testing

	GLenum err;

	// Get errors from OpenGL
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cout << "Error: " << err << std::endl;

	//GLint answer = 0;
	//glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &answer);
	//std::cout << answer << std::endl;

	std::cout << indices.size() << std::endl;

	// Main game loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Poll for events
		glfwPollEvents();

		// Check if ESC pressed, and if so alert window to close
		if (keys[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(mainWindow, GL_TRUE);

		if (keys[GLFW_KEY_LEFT])
		{
			origin.x -= ORIGIN_MOVE / zoom;
			offset.x -= PIXEL_MOVE;
		}
		if (keys[GLFW_KEY_RIGHT])
		{
			origin.x += ORIGIN_MOVE / zoom;
			offset.x += PIXEL_MOVE;
		}
		if (keys[GLFW_KEY_UP])
		{
			origin.y += ORIGIN_MOVE / zoom;
			offset.y += PIXEL_MOVE;
		}
		if (keys[GLFW_KEY_DOWN])
		{
			origin.y -= ORIGIN_MOVE / zoom;
			offset.y -= PIXEL_MOVE;
		}

		if (offset.x >= BLOCK_WIDTH)
		{
			majorOffset.x += 1;
			rendermodeLR = RIGHT;
			offset.x -= BLOCK_WIDTH;
		}
		if (offset.x < 0)
		{
			majorOffset.x -= 1;
			rendermodeLR = LEFT;
			offset.x += BLOCK_WIDTH;
		}
		if (offset.y >= BLOCK_HEIGHT)
		{
			majorOffset.y += 1;
			rendermodeTB = TOP;
			offset.y -= BLOCK_HEIGHT;
		}
		if (offset.y < 0)
		{
			majorOffset.y -= 1;
			rendermodeTB = BOTTOM;
			offset.y += BLOCK_HEIGHT;
		}

		if (majorOffset.x >= BLOCKS_TOTAL_HORIZONTAL)
			majorOffset.x -= BLOCKS_TOTAL_HORIZONTAL;
		if (majorOffset.x < 0)
			majorOffset.x += BLOCKS_TOTAL_HORIZONTAL;
		if (majorOffset.y >=BLOCKS_TOTAL_VERTICAL)
			majorOffset.y -= BLOCKS_TOTAL_VERTICAL;
		if (majorOffset.y < 0)
			majorOffset.y += BLOCKS_TOTAL_VERTICAL;

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
			switch (mode)
			{
			case 1:
				cShader = ComputeShader("shaders/mandelbrot_2/mandelbrot_2_compute.glsl");
				myShader = Shader("shaders/mandelbrot_2/mandelbrot_2_vertex.glsl", "shaders/mandelbrot_2/mandelbrot_2_fragment.glsl");
				std::cout << "Mandelbrot" << std::endl;
				break;
			case 2:
				cShader = ComputeShader("shaders/julia_2/julia_2_compute.glsl");
				myShader = Shader("shaders/julia_2/julia_2_vertex.glsl", "shaders/julia_2/julia_2_fragment.glsl");
				std::cout << "Julia" << std::endl;
				break;
			case 3:
				cShader = ComputeShader("shaders/newton_2/newton_2_compute.glsl");
				myShader = Shader("shaders/newton_2/newton_2_vertex.glsl", "shaders/newton_2/newton_2_fragment.glsl");
				std::cout << "Newton" << std::endl;
				break;
			case 4:
				cShader = ComputeShader("shaders/collatz_2/collatz_2_compute.glsl");
				break;
			}


			c = origin;							// Set Julia set seed ('c') to current origin
			origin = glm::vec2(0.0f, 0.0f);		// Reset origin to (0, 0)
			zoom = 1;							// Reset zoom factor to 1

			bChangeMode = false;
			bRenderAllNow = true;
		}

		if (keys[GLFW_KEY_M] || keys[GLFW_KEY_J] || keys[GLFW_KEY_N] || keys[GLFW_KEY_C])
			bCannotChangeMode = true;
		else
		{
			bCannotChangeMode = false;
			bChangeMode = false;
		}

		// Create the window viewport
		glViewport(0, 0, windowWidth, windowHeight);

		// If bRenderAllNow is true, render the entire screen
		if (bRenderAllNow)
		{
			rendermodeLR = ALL;
			rendermodeTB = NONE;
			bRenderAllNow = false;
		}
		
		normaliser = 1;

		// Compute shader
		if ((rendermodeLR || rendermodeTB))
		{
			// Make sure all data is synchronised
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			cShader.use();

			// Set up all shader variables
			GLint ratioLoc = glGetUniformLocation(cShader.program, "ratio");
			glUniform1f(ratioLoc, ratio);

			GLint zoomLoc = glGetUniformLocation(cShader.program, "zoom");
			glUniform1f(zoomLoc, zoom);			

			GLint maxIterationsLoc = glGetUniformLocation(cShader.program, "maxIterations");
			glUniform1i(maxIterationsLoc, maxIterations);

			GLint offsetLoc = glGetUniformLocation(cShader.program, "offset");
			glm::ivec2 s = glm::ivec2((majorOffset.x * BLOCK_WIDTH), (majorOffset.y * BLOCK_HEIGHT));
			glUniform2i(offsetLoc, s.x, s.y);

			GLint WINDOW_WIDTHLoc = glGetUniformLocation(cShader.program, "WINDOW_WIDTH");
			glUniform1i(WINDOW_WIDTHLoc, WINDOW_WIDTH);

			GLint WINDOW_HEIGHTLoc = glGetUniformLocation(cShader.program, "WINDOW_HEIGHT");
			glUniform1i(WINDOW_HEIGHTLoc, WINDOW_HEIGHT);

			GLint BLOCK_WIDTHLoc = glGetUniformLocation(cShader.program, "BLOCK_WIDTH");
			glUniform1i(BLOCK_WIDTHLoc, BLOCK_WIDTH);

			GLint BLOCK_HEIGHTLoc = glGetUniformLocation(cShader.program, "BLOCK_HEIGHT");
			glUniform1i(BLOCK_HEIGHTLoc, BLOCK_HEIGHT);

			GLint IMAGE_WIDTHLoc = glGetUniformLocation(cShader.program, "IMAGE_WIDTH");
			glUniform1i(IMAGE_WIDTHLoc, IMAGE_WIDTH);

			GLint IMAGE_HEIGHTLoc = glGetUniformLocation(cShader.program, "IMAGE_HEIGHT");
			glUniform1i(IMAGE_HEIGHTLoc, IMAGE_HEIGHT);

			GLint normaliserLoc = glGetUniformLocation(cShader.program, "normaliser");
			glUniform1f(normaliserLoc, normaliser);

			GLint renderOffsetLoc = glGetUniformLocation(cShader.program, "renderOffset");
			GLint originLoc = glGetUniformLocation(cShader.program, "origin");

			// If Julia set, set value of 'c'
			if (mode == 2)
			{
				GLint cLoc = glGetUniformLocation(cShader.program, "c");
				glUniform2f(cLoc, c.x, c.y);
			}

			// Depending on what is being rendered, different render areas must be set
			switch (rendermodeLR)
			{
			case NONE:
				break;
			case ALL:
				glUniform2f(originLoc, origin.x, origin.y);
				glUniform2i(renderOffsetLoc, 0, 0);
				glDispatchComputeGroupSizeARB(BLOCKS_TOTAL_HORIZONTAL, BLOCKS_TOTAL_VERTICAL, 1, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
				break;
			case LEFT:
				glUniform2f(originLoc, origin.x, origin.y);
				glUniform2i(renderOffsetLoc, 0, 0);
				glDispatchComputeGroupSizeARB(1, BLOCKS_TOTAL_VERTICAL, 1, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
				break;
			case RIGHT:
				glUniform2f(originLoc, origin.x, origin.y);
				glUniform2i(renderOffsetLoc, IMAGE_WIDTH - BLOCK_WIDTH, 0);
				glDispatchComputeGroupSizeARB(1, BLOCKS_TOTAL_VERTICAL, 1, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
				break;
			}

			switch (rendermodeTB)
			{
			case NONE:
				break;
			case TOP:
				glUniform2f(originLoc, origin.x, origin.y);
				glUniform2i(renderOffsetLoc, 0, IMAGE_HEIGHT - BLOCK_HEIGHT);
				glDispatchComputeGroupSizeARB(BLOCKS_TOTAL_HORIZONTAL, 1, 1, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
				break;
			case BOTTOM:
				glUniform2f(originLoc, origin.x, origin.y);
				glUniform2i(renderOffsetLoc, 0, 0);
				glDispatchComputeGroupSizeARB(BLOCKS_TOTAL_HORIZONTAL, 1, 1, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
				break;
			}
		}

		// Vertex/fragment shaders
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		myShader.use();
		
		GLint ratioLoc = glGetUniformLocation(myShader.program, "ratio");
		glUniform1f(ratioLoc, ratio);

		GLint zoomLoc = glGetUniformLocation(myShader.program, "zoom");
		glUniform1f(zoomLoc, zoom);

		GLint maxIterationsLoc = glGetUniformLocation(myShader.program, "maxIterations");
		glUniform1i(maxIterationsLoc, maxIterations);

		GLint offsetLoc = glGetUniformLocation(myShader.program, "offset");
		glUniform2i(offsetLoc, offset.x + (majorOffset.x * BLOCK_WIDTH), offset.y + (majorOffset.y * BLOCK_HEIGHT));

		GLint colourOffsetLoc = glGetUniformLocation(myShader.program, "colourOffset");
		glUniform1f(colourOffsetLoc, colourOffset);

		GLint colourScaleLoc = glGetUniformLocation(myShader.program, "colourScale");
		glUniform1f(colourScaleLoc, colourScale);

		GLint WINDOW_WIDTHLoc = glGetUniformLocation(myShader.program, "WINDOW_WIDTH");
		glUniform1i(WINDOW_WIDTHLoc, WINDOW_WIDTH);

		GLint WINDOW_HEIGHTLoc = glGetUniformLocation(myShader.program, "WINDOW_HEIGHT");
		glUniform1i(WINDOW_HEIGHTLoc, WINDOW_HEIGHT);

		GLint BLOCK_WIDTHLoc = glGetUniformLocation(myShader.program, "BLOCK_WIDTH");
		glUniform1i(BLOCK_WIDTHLoc, BLOCK_WIDTH);

		GLint BLOCK_HEIGHTLoc = glGetUniformLocation(myShader.program, "BLOCK_HEIGHT");
		glUniform1i(BLOCK_HEIGHTLoc, BLOCK_HEIGHT);

		GLint IMAGE_WIDTHLoc = glGetUniformLocation(myShader.program, "IMAGE_WIDTH");
		glUniform1i(IMAGE_WIDTHLoc, IMAGE_WIDTH);

		GLint IMAGE_HEIGHTLoc = glGetUniformLocation(myShader.program, "IMAGE_HEIGHT");
		glUniform1i(IMAGE_HEIGHTLoc, IMAGE_HEIGHT);

		GLint normaliserLoc = glGetUniformLocation(myShader.program, "normaliser");
		glUniform1f(normaliserLoc, normaliser);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, vCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap backbuffer to front
		glfwSwapBuffers(mainWindow);

		// Clear colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// For testing triangles one at a time
		if (vCount < indices.size())
		{
			vCount += 3;

			std::cout << vCount << std::endl;
			std::cin.ignore();
		}

		if (bChangeColour)
		{
			//colourOffset = fmod(colourOffset + 0.001f, 1.f);
			colourScale = fmod(colourScale + 0.01f, 100.f);
		}
	}

	return 0;
}