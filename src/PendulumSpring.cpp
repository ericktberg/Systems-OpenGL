#include "PendulumSpring.h"
#include <iostream>

PendulumSpring::PendulumSpring() : DynamicObject()
{
}

PendulumSpring::~PendulumSpring()
{
}

void PendulumSpring::update(float dt) {
	glm::vec3 spring_force = k_*(length_ - rest_length_) * (p2 - p1);
	velocity_ += (spring_force + gravity_) * dt;
	p1 += velocity_ * dt;
	object_->translate(velocity_ * dt);
	length_ = glm::length(p2 - p1);
}

void PendulumSpring::init(RenderableObject* object) {
	object_ = object;
	float x = sqrt(3);
	p1 = { 0, 0, 0 };
	p2 = glm::vec3(x, x, x);
	velocity_ = { 0, 0, 0 };
	rest_length_ = 3;
	length_ = 3;
	k_ = 1000;
}