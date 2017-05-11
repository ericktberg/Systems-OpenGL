#include "Rectangle.h"

Rectangle::
Rectangle(float x, float y, float height, float width, glm::vec4 color) : height_(height), width_(width), RenderableObject(GL_LINES) {
	position_ = { x, y, 0 };

	vertices_.push_back({ { -width / 2, -height / 2, 0 }, color });  // 0
	vertices_.push_back({ { width / 2, -height / 2, 0 }, color });  // 1
	vertices_.push_back({ { width / 2, height / 2, 0 }, color });  // 2
	vertices_.push_back({ { -width / 2, height / 2, 0 }, color });  // 3
	
	

	edges_.push_back({ 0, 3 });
	edges_.push_back({ 3, 2 });
	edges_.push_back({ 2, 1 }); 
	edges_.push_back({ 1, 0 });


	translate(x, y, 0);
}

Rectangle::
Rectangle(float x, float y, float height, float width) : Rectangle(x, y, height, width, WHITE) {


}

Payload Rectangle::
rayCollision(const glm::vec3& velocity, const glm::vec3& origin) const {
	return rectangleIntersection(velocity, origin);
}

Payload Rectangle::
rectangleIntersection(const glm::vec3& velocity, const glm::vec3& origin) const {
	Payload payload = { -1, -1, { 0, 0, 0 } };
	if (!glm::length(velocity)) {
		return payload;
	}
	for (int i = 0; i < edges_.size(); i++) {
		/* ray-line intersection for each edge */
		Edge edge = edges_.at(i);

		float t, u;
		glm::vec3 normal;

		lineIntersection(origin, velocity,
			vertices_.at(edge.p1).position + position_,
			vertices_.at(edge.p2).position + position_,
			&t, &u, &normal);

		if (t >= 0 && t <= 1  && u >= 0 && u <= 1) {
			payload.t = (t < payload.t || payload.t < -.05) ? t : payload.t;
  			payload.normal = normal;
		}

	}
	return payload;
}

float Rectangle::collisionPoint(const glm::vec3& vector, const glm::vec3& position) const {
	return -1;
}

