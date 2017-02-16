#pragma once
#include "DynamicObject.h"
#include "RenderableObject.h"
#include "Spline.h"
#include <glm/glm.hpp>

class PendulumSpring : public DynamicObject
{
public:
	PendulumSpring();
	~PendulumSpring();
	void update(float dt);
	void init(RenderableObject* object);
	void render() const;
private:
	RenderableObject* object_;
	glm::vec3 p1, p2;
	Spline* cord_;
	float k_, rest_length_, length_, mass_, length2_;
	glm::vec3 velocity_, velocity2_;
};

