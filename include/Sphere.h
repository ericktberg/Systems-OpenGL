#pragma once
#include "RenderableObject.h"
#include <glm/glm.hpp>

class Sphere : public RenderableObject
{
public:
	Sphere(float radius, glm::vec3 center, int subdiv);
	~Sphere();
	void init();
	bool intersectsGround(const glm::vec3& ground_plane);
	float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const;
	glm::vec3 center() const { return center_; }
	float radius() const { return radius_; }

private:
	void subdivideTriangle(int face_idx);
	Vertex midPoint(const int p1, const int p2);

	int subdiv_;
	float radius_;
	glm::vec3 center_;
};

