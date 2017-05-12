#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "Payload.h"
#include "Program.h"

#define WHITE {1,1,1,1}
#define BLUE {0, 0, 1, 1}
#define GREEN {0, 1, 0, 1}
#define RED {1, 0, 0, 1}
#define MAGENTA glm::vec4(1, 0, 1, 1)
#define CYAN glm::vec4(0, 1, 1, 1)

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

	int size() const { return vertices_.size(); }
	//----------------------------------------------------------------------------
	// Rendering
	void prepareRender(Program* program) const;
	void render(Program* program, GLenum usage) const;

	//----------------------------------------------------------------------------
	// Transformations
	virtual void translate(const glm::vec3& displacement);
	virtual void translate(glm::mat4 translation) { translation_ = translation; }
	virtual void translate(float x, float y, float z);
	virtual void scale(const glm::vec3& scale);
	virtual void scale(glm::mat4 scale) { scale_ = scale; }
	virtual void rotate(float angle, const glm::vec3& axis);
	virtual void rotate(glm::mat4 rotation) { rotation_ = rotation; }

	glm::vec3 position() const { return position_; }
	glm::mat4 translation() const { return translation_; }
	glm::mat4 scale() const { return scale_; }
	glm::mat4 rotation() const { return rotation_; }

	virtual void interact() {}

	//----------------------------------------------------------------------------
	// Collisions
	virtual bool intersectsGround(const glm::vec3& groundPlane) { return false; }
	virtual Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin) {
		return{ -1, -1, { 0, 0, 0}}; }
	// deprecated
	virtual float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const { return -1; }
	virtual bool within(const glm::vec3& position) const { return false; }

	void setIndex(int idx) { scene_idx_ = idx; };
	int index() const { return scene_idx_; }

protected:
	//----------------------------------------------------------------------------
	// Geometry
	std::vector<Vertex> vertices_;
	std::vector<Face> indices_;
	std::vector<Edge> edges_;

	//----------------------------------------------------------------------------
	// Render
	virtual void renderFaces(Program* program, GLenum usage) const;
	virtual void renderEdges(Program* program, GLenum usage) const;
	virtual void renderPoints(Program* program, GLenum usage) const;
	void renderFan(Program* program, GLenum usage) const;

	GLenum mode_;
	GLuint vbo_, vao_, ebo_;

	//----------------------------------------------------------------------------
	// Transformations
	glm::vec3 position_;
	glm::mat4 translation_;
	glm::mat4 rotation_;
	glm::mat4 scale_;

	glm::vec3 velocity_, acceleration_;

	//----------------------------------------------------------------------------
	// Collisions
	void lineIntersection(
		const glm::vec3& p,
		const glm::vec3& r,
		const glm::vec3& q,
		const glm::vec3& p2,
		float *t,
		float *u,
		glm::vec3 *normal = nullptr) const;

	int scene_idx_;
	//----------------------------------------------------------------------------
	// Properties
	float cor_ = 1;

};

inline float cross(const glm::vec3& v, const glm::vec3& w) {
	return v.x*w.y - v.y*w.x;
}
