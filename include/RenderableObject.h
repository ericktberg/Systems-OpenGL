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
	RenderableObject();
	virtual ~RenderableObject();
	void prepareRender() const;
	virtual void renderFaces(GLenum usage) const;
	virtual void renderEdges(GLenum usage) const;
	virtual void renderPoints(GLenum usage) const;
	virtual void init();
	virtual bool intersectsGround(const glm::vec3& groundPlane) { return false; }
	virtual void translate(const glm::vec3& displacement);

	Program* program() const { return program_; }

protected:
	Program* program_;

	GLuint vbo_, vao_, ebo_;
	glm::mat4 translation_;
	glm::mat4 rotation_;
	glm::mat4 scale_;

	std::vector<Vertex> vertices_;
	std::vector<Face> indices_;
	std::vector<Edge> edges_;
};

