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

glm::vec3 RenderableObject::
calcNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const {
	return glm::normalize(glm::cross((p2 - p1), (p3 - p1)));
}

void RenderableObject::
calcNormals() {
	normal_lines_.clear();
	for (int i = 0; i < vertices_.size(); i++) {
		updateNormal(i, { 0, 0, 0 });
	}

	for (int face_idx = 0; face_idx < indices_.size(); face_idx++) {
		Face f = indices_.at(face_idx);
		glm::vec3 n = calcNormal(point(f.a),
			point(f.b),
			point(f.c));
		incrementNormal(f.a, n);
		incrementNormal(f.b, n);
		incrementNormal(f.c, n);
	}

	for (int i = 0; i < vertices_.size(); i++) {
		Vertex vertex = vertices_.at(i);
		normal_lines_.push_back({ vertex.position, { 0, 0, 1, 1 }, { 0, 0, 0 } });
		normal_lines_.push_back({ vertex.position + 3.f * vertex.normal, { 0, 1, 1, 1 }, { 0, 0, 0 } });
	}
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
	GLint normAttrib = program->getAttribLoc("normal");

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(GLfloat)));
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
// TODO optimize?
//----------------------------------------------------------------------------
// Displace all vertices equally
void RenderableObject::
translate(const glm::vec3& displacement) {
	position_ += displacement;
	translation_ = glm::translate(translation_, displacement );
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
	if (visible_normals_) {
		renderNormals(program, usage);
	}

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
	else {
		std::cerr << "No render mode found." << std::endl;
		return;
	}
}

void RenderableObject::
renderNormals(Program* program, GLenum usage_) const {
	prepareRender(program);

	glBufferData(GL_ARRAY_BUFFER, normal_lines_.size() * sizeof(Vertex), normal_lines_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, normal_lines_.size());
}

/* axes stuff */
//
//// init geometry
//glm::vec3 o_pos = { 0.0f, 0.0f, 0.0f };
//glm::vec3 x_pos = { 1.0f, 0.0f, 0.0f };
//glm::vec3 y_pos = { 0.0f, 1.0f, 0.0f };
//glm::vec3 z_pos = { 0.0f, 0.0f, 1.0f };
//glm::vec4 o_col = { 0.0f, 0.0f, 0.0f, 1 };
//glm::vec4 x_col = { 1.0f, 0.0f, 0.0f, 1 };
//glm::vec4 y_col = { 0.0f, 1.0f, 0.0f, 1 };
//glm::vec4 z_col = { 0.0f, 0.0f, 1.0f, 1 };
//
//
//Vertex origin = { o_pos, o_col };
//Vertex z = { z_pos, z_col };
//Vertex y = { y_pos, y_col };
//Vertex x = { x_pos, x_col };
//vertices_ = { origin, x, y, z };
//edges_ = {
//	{ 0, 1 },
//	{ 0, 2 },
//	{ 0, 3 }
//};