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
			zoom *= 2;
		else if (key == GLFW_KEY_W)
			zoom /= 2;
		else if (key == GLFW_KEY_R)
		{
			zoom = 1;
			origin.x = 0.0f;
			origin.y = 0.0f;
		}
		else if (key == GLFW_KEY_M)
			mode = 1;
		else if (key == GLFW_KEY_J)
			mode = 2;

		//std::cout << key << std::endl;
		//std::cout << mode << std::endl;

		break;
	case GLFW_RELEASE:
		keys.at(key) = false;
		break;
	}
}