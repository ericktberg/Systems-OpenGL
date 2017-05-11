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

#include "Block.h"
#include "Ball.h"
#include "Circle.h"
#include "Paddle.h"
#include "Rectangle.h"

#define PI 3.14159
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 800

Scene scene;
Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
bool active = false;
double lastx, lasty;
bool moved = false;

Paddle* paddle = new Paddle(0, 0, 5);

//----------------------------------------------------------------------------
// function that is called whenever a key is pressed
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {

	case GLFW_KEY_UP:

		break;

	case GLFW_KEY_DOWN:
	
		break;

	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) {
			active = !active;
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


		lastx = xpos;
		lasty = ypos;
		moved = true;
}
//----------------------------------------------------------------------------
// function that is called whenever a mouse or trackpad button press event occurs
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
}

//----------------------------------------------------------------------------
// GLFW window stuff
void createWindow(GLFWwindow** window, int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	*window = glfwCreateWindow(width, height, "test", nullptr, nullptr);  // Windowed
	glfwMakeContextCurrent(*window);
}

int main() {

	auto t_start = std::chrono::high_resolution_clock::now();
	/**********************************
	* Initialize library elements
	***********************************/
	GLFWwindow *window;

	createWindow(&window, WINDOW_WIDTH, WINDOW_HEIGHT);
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
	glClearColor(0, 0, 0, 1);
	/**********************************
	* Define GLFW input callbacks
	***********************************/
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	/**********************************
	* Define buffer objects
	***********************************/
	int constant = scene.addShader(new Program(CONSTANT_COL));
	int paddleID = scene.addObject(paddle);

	scene.assignShader(paddleID, constant);

	/* Define game boundaries */
	int left = scene.addObject(new Rectangle(-40, 0, 41, 1));
	scene.assignShader(left, constant);
	int right = scene.addObject(new Rectangle(40, 0, 41, 1));
	scene.assignShader(right, constant);
	int top = scene.addObject(new Rectangle(0, -20, 1, 81));
	scene.assignShader(top, constant);
	int bottom = scene.addObject(new Rectangle(0, 20, 1, 81));
	scene.assignShader(bottom, constant);
	
	/* Create Bricks */
	for (int i = 0; i < 26; i++) {
		float x = -38 + i * 3;
		int block;

		block = scene.addObject(new Block(x, 0, .9, 2.9));

		scene.assignShader(block, constant);

	}
	/*
	for (int i = 0; i < 25; i++) {
		float x = -36 + i * 3;
		int block;

		block = scene.addObject(new Block(x, 1, .9, 2.9));
		scene.assignShader(block, constant);
	}
	*/

	paddle->scene = &scene;

	/* Create ball and paddle*/
	Ball* ball = new Ball(0, 10, .3);
	int ball_idx  = scene.addObject(ball);
	scene.assignShader(ball_idx, constant);

	auto t_now = t_start, t_last = t_start;
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
		//std::cerr << "elapsed: " << time << std::endl;
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
		ball->start(active);

		scene.render(camera);

		glfwSwapBuffers(window);
		paddle->translate(lastx * 80 / WINDOW_WIDTH - 40, -lasty * 40 / WINDOW_HEIGHT + 20, 0);
		moved = false;
		ball->update(time > .03 ? .03 : time, &scene);
		/**********************************
		* Limit framerate
		***********************************/
	
		auto t_end = std::chrono::high_resolution_clock::now();
	}


	glfwDestroyWindow(window);
	double fps = frames / total_time;
	std::cerr << "\nAverage FPS: " << fps << std::endl;

	glfwTerminate();
	exit(EXIT_SUCCESS);

}