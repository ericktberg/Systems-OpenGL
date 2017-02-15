#include "callbacks.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define MOUSE_DRAG 1
#define CONTROL_HELD 2

namespace callbacks {
	int state = 0;

	//----------------------------------------------------------------------------
	// function that is called whenever a keyboard event occurs; defines how keyboard input will be handled
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//	switch (key) {
	//	case GLFW_KEY_RIGHT:
	//		if (action == GLFW_PRESS){
	//			printf("I pressed the right key.\n");
	//		}
	//		break;


	//	case GLFW_KEY_LEFT:
	//		if (action == GLFW_PRESS){
	//			printf("i pressed the left key.\n");
	//		}
	//		break;

	//	case GLFW_KEY_UP:
	//		if (action == GLFW_PRESS){
	//			printf("I pressed the up key.\n");
	//		}
	//		break;

	//	case GLFW_KEY_DOWN:
	//		if (action == GLFW_PRESS){
	//			printf("I pressed the down key.\n");
	//		}
	//		break;

	//	case GLFW_KEY_R:
	//		if (action == GLFW_PRESS){
	//			printf("I pressed the R key.\n");
	//		}
	//		break;

	//	case GLFW_KEY_SPACE:
	//		printf("space pressed.\n");
	//		if (action == GLFW_PRESS){
	//			printf("I pressed the control down at state ==0.\n");
	//		}
	//		else if (action == GLFW_RELEASE) {
	//		}
	//		break;

	//	case 033:  // octal ascii code for ESC
	//	case 'q':
	//	case 'Q':
	//		glfwSetWindowShouldClose(window, GL_TRUE);
	//		break;
	//	}
	//}

	////----------------------------------------------------------------------------
	//// function that is called whenever a mouse or trackpad button press event occurs
	//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	//	glfwGetCursorPos(window, &mouse_x_g, &mouse_y_g);

	//	// Check which mouse button triggered the event, e.g. GLFW_MOUSE_BUTTON_LEFT, etc.
	//	// and what the button action was, e.g. GLFW_PRESS, GLFW_RELEASE, etc.
	//	// (Note that ordinary trackpad click = mouse left button)
	//	// Also check if any modifier keys were active at the time of the button press, e.g. GLFW_MOD_ALT, etc.
	//	// Take the appropriate action, which could (optionally) also include changing the cursor's appearance
	//	if (button == GLFW_MOUSE_BUTTON_LEFT && action ==
	//		GLFW_PRESS) {
	//	}
	//	else if (button == GLFW_MOUSE_BUTTON_LEFT && action ==
	//		GLFW_RELEASE) {
	//	}
	//}

	//

}