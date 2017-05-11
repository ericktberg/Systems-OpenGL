#pragma once

#include "Payload.h"
#include "RenderableObject.h"

class Rectangle : public RenderableObject {
public:
	Rectangle(float x, float y, float height, float width, glm::vec4 color);
	Rectangle(float x, float y, float height, float width);

	virtual Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin) const;
	float collisionPoint(const glm::vec3& vector, const glm::vec3& position) const;
	Payload rectangleIntersection(const glm::vec3& velocity, const glm::vec3& origin) const;

private:
	float height_, width_;

};

