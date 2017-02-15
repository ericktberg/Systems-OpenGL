#pragma once
#include "DynamicObject.h"
#include "RenderableObject.h"
#include <glm/glm.hpp>

class PendulumSpring : public DynamicObject
{
public:
	PendulumSpring();
	~PendulumSpring();
	void update(float dt);
	void init(RenderableObject* object);

private:
	RenderableObject* object_;
	glm::vec3 p1, p2;
	float k_, rest_length_, length_, ball_mass_;
	glm::vec3 velocity_;
};

