#pragma once

#include "RenderableObject.h"

class Circle : public RenderableObject {
public:
	Circle(float x, float y, float radius);

	Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin);
	float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const;

private:
	float radius_;
};