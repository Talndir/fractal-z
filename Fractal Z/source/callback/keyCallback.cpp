// ===============================================================
/* Includes */
#include "../../header/callback/keyCallback.h"	// Header
#include "../../header/external.h"		// For keys and mode

#include <vector>
#include <iostream>

// ===============================================================
/* Definition of keycallback function */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (action)
	{
	case GLFW_PRESS:
		keys.at(key) = true;

		if (key == GLFW_KEY_Q)
		{
			zoom *= 2;
			rendermodeLR = ALL;
			bRenderAllNow = true;
		}
		else if (key == GLFW_KEY_W)
		{
			zoom /= 2;
			rendermodeLR = ALL;
			bRenderAllNow = true;
		}
		else if (key == GLFW_KEY_R)
		{
			zoom = 1;
			origin = glm::vec2(0.0f, 0.0f);
			offset = glm::ivec2(0, 0);
			majorOffset = glm::ivec2(0, 0);
			colourOffset = 0.0f;
			colourScale = 1.0f;
			rendermodeLR = ALL;
			bRenderAllNow = true;
		}
		else if (key == GLFW_KEY_M)
			mode = 1;
		else if (key == GLFW_KEY_J)
			mode = 2;
		else if (key == GLFW_KEY_N)
			mode = 3;
		else if (key == GLFW_KEY_C)
			mode = 4;
		else if (key == GLFW_KEY_SPACE)
			bChangeColour = !bChangeColour;

		break;
	case GLFW_RELEASE:
		keys.at(key) = false;
		break;
	}
}