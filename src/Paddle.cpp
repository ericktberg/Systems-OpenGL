#include "Paddle.h"

Paddle::
Paddle(float x, float y, float width) : RenderableObject(GL_LINES) {
	vertices_.push_back({ { -width / 2, 0, 0 }, { 0, 1, 0, 1 } });
	vertices_.push_back({ { width / 2, 0, 0 }, { 0, 1, 0, 1 } });
	translate(x, y, 0);

	edges_.push_back({ 0, 1 });
}

Payload Paddle::
rayCollision(const glm::vec3& velocity, const glm::vec3& origin) const {
	float t = -1, u = -1;
	glm::vec3 normal;
	Payload payload = { -1, -1 };

	if (glm::length(velocity)) {

		glm::vec3 v1 = vertices_.at(0).position + position_;
		glm::vec3 v2 = vertices_.at(1).position + position_;
		lineIntersection(velocity , origin, v1, v2, &t, &u);

		glm::vec3 test = origin + t * velocity;
	
		payload.velocity = velocity_;
		if (t > 0 && t < 1 && u > 0 && u < 1) {
			int sign = origin.y > position_.y ? 1 : -1;
			payload.normal = { 0, sign * 1, 0 };
			glm::vec4 color = sign > 0 ? MAGENTA : CYAN;
			scene->assignShader(scene->addObject(new Block(v1.x, v1.y, .5, .5, POPPING, GREEN)), 0);
			scene->assignShader(scene->addObject(new Block(v2.x, v2.y, .5, .5, POPPING, BLUE)), 0);
			scene->assignShader(scene->addObject(new Block(test.x, test.y, .2, .2, POPPING, color)), 0);

			payload.t = t;
			payload.id = index();
		}
	}


	return payload;
}

void Paddle::
translate(float x, float y, float z) {

	velocity_ = glm::vec3(x - position_.x, y - position_.y, z - position_.z) / .03f;

	position_ = { x, y, z };
	translation_ = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, -y, z, 1 } };
}