#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Program.h"

class RenderableObject
{
public:
	RenderableObject(int type);
	virtual ~RenderableObject();
	void prepareRender() const;
	virtual void renderFaces(GLenum usage) const;
	virtual void renderEdges(GLenum usage) const;
	virtual void renderPoints(GLenum usage) const;
	virtual void init();
	virtual bool intersectsGround(const glm::vec3& groundPlane) { return false; }
	virtual void translate(const glm::vec3& displacement);
	virtual void translate(glm::mat4 translation) { translation_ = translation; }

	int size() const { return vertices_.size(); }

	void movePoint(int idx, glm::vec3 displacement) { vertices_.at(idx).position += displacement; }
	glm::vec3 point(int idx) const { return vertices_.at(idx).position; }
	glm::vec3 position() const { return position_; }

	glm::mat4 translation() const { return translation_; }


	Program* program() const { return program_; }

protected:
	Program* program_;

	GLuint vbo_, vao_, ebo_;

	glm::vec3 position_;
	glm::mat4 translation_;
	glm::mat4 rotation_;
	glm::mat4 scale_;

	std::vector<Vertex> vertices_;
	std::vector<Face> indices_;
	std::vector<Edge> edges_;
};

