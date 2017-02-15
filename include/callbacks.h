#include <GLFW/glfw3.h>

namespace callbacks {

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	void createWindow(GLFWwindow** window, int height, int width);

}