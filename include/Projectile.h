#pragma once
#include <glm/glm.hpp>

#include "DynamicObject.h"
#include "RenderableObject.h"

class Projectile : public DynamicObject {
public: 
	Projectile(glm::vec3 init_velocity, glm::vec3 gravity);
	Projectile(glm::vec3 init_velocity);
	~Projectile();
	void init(RenderableObject* object);
	void update(float dt);

private:
	RenderableObject* object_;

	glm::vec3 velocity_;
};