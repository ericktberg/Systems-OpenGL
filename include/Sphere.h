#pragma once
#include "RenderableObject.h"
#include <glm/glm.hpp>

class Sphere : public RenderableObject
{
public:
	//----------------------------------------------------------------------------
	// Constructors
	Sphere(GLenum mode, float radius, glm::vec3 center, int subdiv, glm::vec4 color);
	Sphere(GLenum mode, float radius, glm::vec3 center, int subdiv);
	Sphere(GLenum mode, float radius, glm::vec3 center);
	Sphere(GLenum mode, float radius);
	Sphere(GLenum mode);

	~Sphere();

	//----------------------------------------------------------------------------
	// Collision
	bool intersectsGround(const glm::vec3& ground_plane);
	float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const;

	glm::vec3 normal(glm::vec3 point) {
		return glm::normalize(point - center_);
	}
	
	//----------------------------------------------------------------------------
	// Parameters
	glm::vec3 center() const { return center_; }
	float radius() const { return radius_; }

private:
	//----------------------------------------------------------------------------
	// Geometry Construction
	void subdivideTriangle(int face_idx);
	Vertex midPoint(const int p1, const int p2);

	//----------------------------------------------------------------------------
	// Sphere Parameters
	int subdiv_;
	float radius_;
	glm::vec3 center_;
};

