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
	RenderableObject(GLenum mode);
	virtual ~RenderableObject();

	virtual void vaoInit(Program* program);
	//----------------------------------------------------------------------------
	// Geometry
	glm::vec3 point(int idx) const { return vertices_.at(idx).position; }
	void editPoint(int idx, glm::vec3 newPoint) { vertices_.at(idx).position = newPoint; }

	virtual glm::vec3 normal(glm::vec3 point) {	return{ 0, 0, 0 }; }

	void updateNormal(int idx, glm::vec3 normal) { vertices_.at(idx).normal = normal; }
	void incrementNormal(int idx, glm::vec3 normal) 
		{ vertices_.at(idx).normal = glm::normalize(vertices_.at(idx).normal + normal); }

	int size() const { return vertices_.size(); }
	//----------------------------------------------------------------------------
	// Rendering
	void prepareRender(Program* program) const;
	void render(Program* program, GLenum usage) const;

	void normalVisible(bool state) { visible_normals_ = state; }
	void calcNormals();
	glm::vec3 calcNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const;


	//----------------------------------------------------------------------------
	// Transformations
	virtual void translate(const glm::vec3& displacement);
	virtual void translate(glm::mat4 translation) { translation_ = translation; }
	virtual void scale(const glm::vec3& scale);
	virtual void scale(glm::mat4 scale) { scale_ = scale; }
	virtual void rotate(float angle, const glm::vec3& axis);
	virtual void rotate(glm::mat4 rotation) { rotation_ = rotation; }

	glm::vec3 position() const { return position_; }
	glm::mat4 translation() const { return translation_; }
	glm::mat4 scale() const { return scale_; }
	glm::mat4 rotation() const { return rotation_; }

	//----------------------------------------------------------------------------
	// Collisions
	virtual bool intersectsGround(const glm::vec3& groundPlane) { return false; }
	virtual float rayCollision(const glm::vec3& velocity, const glm::vec3& origin) { return -1; }
	// deprecated
	virtual float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const { return -1; }
	virtual bool within(const glm::vec3& position) const { return false; }


protected:
	//----------------------------------------------------------------------------
	// Geometry
	std::vector<Vertex> vertices_;
	std::vector<Face> indices_;
	std::vector<Edge> edges_;

	bool visible_normals_;
	std::vector<Vertex> normal_lines_;
	//----------------------------------------------------------------------------
	// Render
	virtual void renderFaces(Program* program, GLenum usage) const;
	virtual void renderEdges(Program* program, GLenum usage) const;
	virtual void renderPoints(Program* program, GLenum usage) const;
	virtual void renderNormals(Program* program, GLenum usage) const;

	GLenum mode_;
	GLuint vbo_, vao_, ebo_;

	//----------------------------------------------------------------------------
	// Transformations
	glm::vec3 position_;
	glm::mat4 translation_;
	glm::mat4 rotation_;
	glm::mat4 scale_;

	//----------------------------------------------------------------------------
	// Collisions
	
};

