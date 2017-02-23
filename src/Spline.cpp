/****************************************************************************************
* Currently a shell
* Will have bezier capabilities and other spline things.
*****************************************************************************************/

#include "Spline.h"
// TODO: spline and pendulum are broken now
Spline::Spline(glm::vec3 p1, glm::vec3 p2, glm::vec4 color) : RenderableObject(GL_LINES) {
	vertices_.push_back({ p1, color });
	vertices_.push_back({ p2, color });
	edges_.push_back({ 0, 1 });
}

Spline::~Spline() {

}

void Spline::
vaoInit(Program* program) {
	// init program
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

	GLint posAttrib = program->getAttribLoc("position");
	GLint colAttrib = program->getAttribLoc("color");

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
}

void Spline::renderFaces(GLenum usage) const {
	return;
}

glm::vec3 Spline::vector() const {
	return vertices_.at(vertices_.size() - 1).position - vertices_.at(0).position;
}