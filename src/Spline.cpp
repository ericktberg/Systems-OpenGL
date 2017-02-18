/****************************************************************************************
* Currently a shell
* Will have bezier capabilities and other spline things.
*****************************************************************************************/

#include "Spline.h"

Spline::Spline(glm::vec3 p1, glm::vec3 p2, glm::vec4 color) : RenderableObject(1) {
	vertices_.push_back({ p1, color });
	vertices_.push_back({ p2, color });
	edges_.push_back({ 0, 1 });
}

Spline::~Spline() {

}

void Spline::init() {

}

void Spline::renderFaces(GLenum usage) const {
	return;
}

glm::vec3 Spline::vector() const {
	return vertices_.at(vertices_.size() - 1).position - vertices_.at(0).position;
}

void Spline::movePoint(int idx, glm::vec3 displacement) {
	vertices_.at(idx).position += displacement;
}