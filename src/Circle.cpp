#include "Circle.h"

#define PI 3.14159
Circle::
Circle(float x, float y, float radius) : radius_(radius), RenderableObject(GL_TRIANGLE_FAN) {
	int subdivs = 20;
	glm::vec4 white = { 1, 1, 1, 1 };
	vertices_.push_back({ { 0, 0, 0 }, white });

	for (int i = 0; i <= subdivs; i++) {
		vertices_.push_back({ 
			{ radius * cos(i * 2 * PI / subdivs), 
			  radius * sin(i * 2 * PI / subdivs), 0 }, white });
	}

	translate(x, y, 0);
}

Payload Circle::
rayCollision(const glm::vec3& velocity, const glm::vec3& origin) {
	//TODO
	return{ -1, -1, { 0, 0, 0 } };
}

float Circle::
collisionPoint(const glm::vec3& vector, const glm::vec3& position) const {
	//TODO
	return -1;
}