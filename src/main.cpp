#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>
#include <thread>

#include "Camera.h"
#include "Scene.h"

#include "Digraph.h"
#include "Plane.h"
#include "Sphere.h"
#include "Spline.h"
#include "RenderableObject.h"

#include "Cloth.h"
#include "PendulumSpring.h"
#include "Projectile.h"
#include "ShallowWater.h"

#define PI 3.14159
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

bool active = false;

GLdouble mouse_x_g, mouse_y_g;
bool mouse_drag_g = false;
Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

Scene scene;

static void diffuse() {
	Program* new_program = new Program(0);
	scene.addShader(new_program);
}

static void object() {
	Program* new_program = new Program(1);
	scene.addShader(new_program);
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

	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS){
			active ^= true;
		}
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

void clothSim() {
	int phong = scene.addShader(new Program(PHONG_NOSPEC));
	int constant = scene.addShader(new Program(CONSTANT_COL));

	int sphere = scene.addObject(new Sphere(GL_TRIANGLES, .5, { 0, 0, 2 }, 3));
	scene.assignShader(sphere, phong);

	Plane* plane = new Plane(GL_TRIANGLES, 2, 2, { .4, .1, .4, 1 }, 20, 20);
	plane->calcNormals();
	int plane_idx = scene.addObject(plane);
	scene.translate(plane_idx, { -1, 0, 3 });
	scene.assignShader(plane_idx, phong);

	int ground = scene.addObject(new Plane(GL_LINES, 20, 20, { .6, .2, .2, 1 }, 20, 20));
	scene.assignShader(ground, constant);

	Cloth cloth(plane);

	int ball = scene.addObject(new Sphere(GL_POINTS, .25, { 0, 3, 2 }, 3));
	scene.assignShader(ball, constant);
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
	glClearColor(.95, .93, .93, 1);
	/**********************************
	* Define GLFW input callbacks
	***********************************/
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/**********************************
	* Define buffer objects
	***********************************/
	int phong = scene.addShader(new Program(PHONG_NOSPEC));
	int constant = scene.addShader(new Program(CONSTANT_COL));

	/* floor */
	int plane_idx = scene.addObject(new Plane(GL_TRIANGLES, 100, 100, { .6, .6, .6, 1 }, 1, 1));
	scene.assignShader(plane_idx, phong);

	int sphere1_idx = scene.addObject(
		new Sphere(GL_TRIANGLES, 5, { 0, 30, 0 }, 2, { .2f, .8f, 0.f, 1 }));
	scene.assignShader(sphere1_idx, phong);

	int sphere2_idx = scene.addObject(
		new Sphere(GL_TRIANGLES, 10, { 10, -30, 0 }, 3, { .2f, .4f, 6.f, 1 }));
	scene.assignShader(sphere2_idx, phong);

	int sphere3_idx = scene.addObject(
		new Sphere(GL_TRIANGLES, 20, { 10, 10, 0 }, 4, { .6f, .4f, 3.f, 1 }));
	scene.assignShader(sphere3_idx, phong);

	int sphere4_idx = scene.addObject(
		new Sphere(GL_TRIANGLES, 15, { -30, -20, 0 }, 4, { .8f, .2f, 1.f, 1 }));
	scene.assignShader(sphere4_idx, phong);

	int graph = scene.addObject(
		new Digraph());
	scene.assignShader(graph, constant);


	auto t_now = t_start, t_last = t_start;
	camera.setCamera();
	int frames = 0;
	double total_time = 0;
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
		scene.render(camera);
		glfwSwapBuffers(window);
		/**********************************
		* Limit framerate
		***********************************/
		if (active){

		}
		auto t_end = std::chrono::high_resolution_clock::now();
	}


	glfwDestroyWindow(window);
	double fps = frames / total_time;
	std::cerr << "\nAverage FPS: " << fps << std::endl;
	//std::this_thread::sleep_for(std::chrono::seconds(5));

	glfwTerminate();
	exit(EXIT_SUCCESS);

}