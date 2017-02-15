#include "RenderableObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

RenderableObject::RenderableObject() {
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	Shader vertex("shaders/Vertex/simple3d.glsl", GL_VERTEX_SHADER);
	Shader fragment("shaders/Fragment/simpleFragment.glsl", GL_FRAGMENT_SHADER);
	std::vector<Shader> s = { vertex, fragment };
	program_ = new Program(s);

	GLint posAttrib = program_->getAttribLoc("position");
	GLint colAttrib = program_->getAttribLoc("color");

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
}

RenderableObject::~RenderableObject() {
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glDeleteVertexArrays(1, &vao_);
}

//----------------------------------------------------------------------------
// Create axes by default
void RenderableObject::init() {
	glm::vec3 o_pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 x_pos = { 1.0f, 0.0f, 0.0f };
	glm::vec3 y_pos = { 0.0f, 1.0f, 0.0f };
	glm::vec3 z_pos = { 0.0f, 0.0f, 1.0f };
	glm::vec4 o_col = { 0.0f, 0.0f, 0.0f, 1 };
	glm::vec4 x_col = { 1.0f, 0.0f, 0.0f, 1 };
	glm::vec4 y_col = { 0.0f, 1.0f, 0.0f, 1 };
	glm::vec4 z_col = { 0.0f, 0.0f, 1.0f, 1 };

	
	Vertex origin = { o_pos, o_col};
	Vertex z = { z_pos,z_col };
	Vertex y = { y_pos, y_col };
	Vertex x = { x_pos, x_col };
	vertices_ = { origin, x, y, z };
	edges_ = {
		{ 0, 1 },
		{ 0, 2 },
		{ 0, 3 }
	};
}

void RenderableObject::prepareRender() const {
	program_->bind();
	program_->setUniformMatrix4fv("model", glm::value_ptr(scale_ * rotation_ * translation_));

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);
}

//TODO: add error checking on sizes when rendering.
//----------------------------------------------------------------------------
// Render model using GL_TRIANGLES
void RenderableObject::renderFaces(GLenum usage) const {
	prepareRender();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(Face), indices_.data(), GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, indices_.size() * 3, GL_UNSIGNED_INT, (void*)0);
}
//----------------------------------------------------------------------------
// Render model using GL_LINES
void RenderableObject::renderEdges(GLenum usage) const {
	prepareRender();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges_.size() * sizeof(Edge), edges_.data(), usage);
	glDrawElements(GL_LINES, edges_.size() * 2, GL_UNSIGNED_INT, (void*)0);
}
//----------------------------------------------------------------------------
// Render model using GL_POINTS
void RenderableObject::renderPoints(GLenum usage) const {
	prepareRender();

	glDrawArrays(GL_POINTS, 0, vertices_.size());
}
// TODO optimize?
//----------------------------------------------------------------------------
// Displace all vertices equally
void RenderableObject::translate(const glm::vec3& displacement) {
	glm::translate(translation_, displacement );
}