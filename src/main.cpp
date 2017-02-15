#include <stdio.h>
#include <stdlib.h>
#define _GLFW_WIN32

#include <GL/glew.h>

#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Camera.h"
#include "Projectile.h"
#include "PendulumSpring.h"
#include "RenderableObject.h"
#include "Sphere.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define PI 3.14159265359

static GLdouble mouse_x_g, mouse_y_g;
static bool mouse_drag_g = false;

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

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
static void createWindow(GLFWwindow** window, int height, int width) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	*window = glfwCreateWindow(width, height, "Cloth Simulation", nullptr, nullptr);  // Windowed
	glfwMakeContextCurrent(*window);
}



int main(void) {
	auto t_start = std::chrono::high_resolution_clock::now();
	/**********************************
	* Initialize library elements
	***********************************/
	GLFWwindow *window;

	glewExperimental = GL_TRUE;
	createWindow(&window, WINDOW_HEIGHT, WINDOW_WIDTH);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	glfwMakeContextCurrent(window);
	/**********************************
	* Define GLFW input callbacks
	***********************************/
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/**********************************
	* Enable openGL processing features
	***********************************/
	glEnable(GL_BLEND); glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/**********************************
	* Setup camera transformations
	***********************************/
	camera.setCamera();
	/**********************************
	* Create objects and populate
	***********************************/
	RenderableObject axes;
	axes.init();

	//Sphere sphere(.3, { 1.f, 1.f, 2.f }, 3);
	//sphere.init();
	//PendulumSpring test;
	//test.init(&sphere);
	/**********************************
	* Dat main loop tho
	***********************************/
	int frames = 0;
	double total_time = 0;
	auto t_now = t_start, t_last = t_start;
	while (!glfwWindowShouldClose(window)) {
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
		camera.setCamera();
		camera.renderEdgesFrom(axes);
		//camera.renderEdgesFrom(sphere);
		glfwSwapBuffers(window);
		/**********************************
		* Update any elements at the end of the loop
		***********************************/
		//dyna.update(time > .033 ? .033 : time);
		//test.update(time > .02 ? .02 : time);
		auto t_end = std::chrono::high_resolution_clock::now();
	}
	/**********************************
	* Cleanup
	***********************************/
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
