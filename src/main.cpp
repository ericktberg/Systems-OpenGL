#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>

#include "Camera.h"
#include "Sphere.h"
#include "RenderableObject.h"

#define PI 3.14159
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


GLdouble mouse_x_g, mouse_y_g;
bool mouse_drag_g = false;
Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
glm::mat4 view;
Vertex axes[] = {
	{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1 } },
	{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1 } },  // Z axis - Blue
	{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1 } },
	{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1 } },  // Y axis - Green
	{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1 } },
	{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1 } }  // X axis - Red
};

void resetCamera() {
	glm::vec3 camera_pos = {
		cos(camera.rotV() * PI / 180) * sin(camera.rotU() * PI / 180) * camera.dolly(),
		sin(camera.rotV() * PI / 180) * sin(camera.rotU() * PI / 180) * camera.dolly(),
		camera.dolly() * cos(camera.rotU() * PI / 180)
	};
	view = glm::lookAt(
		camera_pos,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f)
		);
}

//----------------------------------------------------------------------------
// function that is called whenever a key is pressed
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {

	case GLFW_KEY_UP:
		if (action == GLFW_PRESS){
			camera.dollyInc(-1);
		}
		else if (action == GLFW_REPEAT) {
			camera.dollyInc(-.5);
		}
		camera.setCamera();
		break;

	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS){
			camera.dollyInc(1);
		}
		else if (action == GLFW_REPEAT) {
			camera.dollyInc(.5);
		}
		camera.setCamera();
		break;
	case 033:  // octal ascii code for ESC
	case 'q':
	case 'Q':
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	}

}

//----------------------------------------------------------------------------
// function that is called whenever a cursor motion event occurs
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	double dx, dy;
	dx = xpos - mouse_x_g;
	dy = ypos - mouse_y_g;
	mouse_x_g = xpos;
	mouse_y_g = ypos;
	if (mouse_drag_g) {
		camera.rotV(dx);
		camera.rotU(-dy);
		camera.setCamera();
	}
}
//----------------------------------------------------------------------------
// function that is called whenever a mouse or trackpad button press event occurs
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	glfwGetCursorPos(window, &mouse_x_g, &mouse_y_g);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action ==
		GLFW_PRESS) {
		mouse_drag_g = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action ==
		GLFW_RELEASE) {
		mouse_drag_g = false;
	}
}

//----------------------------------------------------------------------------
// GLFW window stuff
void createWindow(GLFWwindow** window, int height, int width) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	*window = glfwCreateWindow(height, width, "test", nullptr, nullptr);  // Windowed
	glfwMakeContextCurrent(*window);
}


int main() {
	auto t_start = std::chrono::high_resolution_clock::now();
	/**********************************
	* Initialize library elements
	***********************************/
	GLFWwindow *window;

	createWindow(&window, WINDOW_HEIGHT, WINDOW_WIDTH);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	/**********************************
	* Enable openGL processing features
	***********************************/
	glEnable(GL_BLEND); glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/**********************************
	* Define GLFW input callbacks
	***********************************/
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/**********************************
	* Define buffer objects
	***********************************/
	RenderableObject axes;
	axes.init();
	Sphere sphere(.25, { 0, 0, 1 }, 3);
	sphere.init();

	int frames = 0;
	double total_time = 0;
	auto t_now = t_start, t_last = t_start;
	camera.setCamera();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		/**********************************
		* Elapsed time previous loop
		***********************************/
		t_last = t_now;
		t_now = std::chrono::high_resolution_clock::now();
		double time = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_last).count();
		std::cerr << "elapsed: " << time << std::endl;
		total_time += time;
		frames++;
		/**********************************
		* Catch graphics processing
		***********************************/
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/**********************************
		* Draw our objects
		***********************************/
		camera.renderEdgesFrom(axes);
		camera.renderPointsFrom(sphere);
		glfwSwapBuffers(window);
		/**********************************
		* Limit framerate
		***********************************/
		auto t_end = std::chrono::high_resolution_clock::now();
	}

	//glDeleteProgram(shader_program);
	//glDeleteShader(fragment_shader);
	//glDeleteShader(vertex_shader);

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

}