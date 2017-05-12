#include "RenderableObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"


#include <iostream>

RenderableObject::
RenderableObject(GLenum mode) {
	mode_ = mode;
}

RenderableObject::
~RenderableObject() {
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glDeleteVertexArrays(1, &vao_);
}

//----------------------------------------------------------------------------
// Create axes by default
void RenderableObject::
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

void RenderableObject::
prepareRender(Program* program) const {
	program->setUniformMatrix4fv("model", glm::value_ptr(translation_ *  rotation_ * scale_ ));
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

//TODO: add error checking on sizes when rendering.
//----------------------------------------------------------------------------
// Render model using GL_TRIANGLES
void RenderableObject::renderFaces(Program* program, GLenum usage) const {
	prepareRender(program);
	
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(Face), indices_.data(), GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, indices_.size() * 3, GL_UNSIGNED_INT, (void*)0);
}
//----------------------------------------------------------------------------
// Render model using GL_LINES
void RenderableObject::renderEdges(Program* program, GLenum usage) const {
	prepareRender(program); 
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges_.size() * sizeof(Edge), edges_.data(), usage);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);

	glDrawElements(GL_LINES, edges_.size() * 2, GL_UNSIGNED_INT, (void*)0);
}
//----------------------------------------------------------------------------
// Render model using GL_POINTS
void RenderableObject::
renderPoints(Program* program, GLenum usage) const {
	prepareRender(program);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges_.size() * sizeof(Edge), edges_.data(), usage);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, vertices_.size());
}

//----------------------------------------------------------------------------
// Render model using GL_POINTS
void RenderableObject::
renderFan(Program* program, GLenum usage) const {
	prepareRender(program);

	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_.size());
}

// TODO optimize?
//----------------------------------------------------------------------------
// Displace all vertices equally
void RenderableObject::
translate(const glm::vec3& displacement) {
	position_ += displacement;
	translation_ = glm::translate(translation_, displacement );
}

void RenderableObject::
translate(float x, float y, float z) {
	position_ = { x, y, z };
	translation_ = { 
		{1, 0, 0, 0},
		{0, -1, 0, 0},
		{0, 0, 1, 0},
		{x, y, z, 1} };
}

void RenderableObject::
scale(const glm::vec3& scale) {
	scale_ = glm::scale(scale_, scale);
}

void RenderableObject::
rotate(float angle, const glm::vec3& axis) {
	rotation_ = glm::rotate(rotation_, angle, axis);
}

void RenderableObject::render(Program* program, GLenum usage) const {
	if (mode_ == GL_POINTS) {
		renderPoints(program, usage);
	}
	else if (mode_ == GL_LINES) {
		renderEdges(program, usage);
	}
	else if (mode_ == GL_TRIANGLES) {
		renderFaces(program, usage);
	}
	else if (mode_ == GL_TRIANGLE_STRIP) {
		// implement tri strips
	}
	else if (mode_ == GL_TRIANGLE_FAN) {
		renderFan(program, usage);
	}
	else {
		std::cerr << "No render mode found." << std::endl;
		return;
	}
}




void RenderableObject::
lineIntersection(
		const glm::vec3& v,
		const glm::vec3& o,
		const glm::vec3& p1,
		const glm::vec3& p2,
		float *t,
		float *u,
		glm::vec3 *normal) const
{
	
	glm::vec3 s = p2 - p1;

	*u = (-v.y * (o.x - p1.x) + v.x * (o.y - p1.y)) / cross(v, s);
	*t = (-s.y * (o.x - p1.x) + s.x * (o.y - p1.y)) / cross(v, s);
	if (*t < 1 && *t > 0) {
		auto breakpoint = 1;
	}
	if (normal) {
		*normal = glm::normalize(glm::vec3(-s.y, s.x, 0));
	}
}