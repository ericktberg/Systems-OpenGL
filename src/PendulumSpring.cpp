
//	glm::vec3 cord_vector = cord_->vector();
//	glm::vec3 spring_force = k_*(length_ - rest_length_) * cord_vector;
//	velocity_ += (spring_force + gravity_) * dt;
//	cord_->movePoint(0, velocity_ * dt);
//	object_->translate(velocity_ * dt);
//	length_ = glm::length(cord_vector);


#include "PendulumSpring.h"
#include <iostream>

PendulumSpring::PendulumSpring(RenderableObject* object, Spline* spline) : DynamicObject()
{
	object_ = object;
	float x = sqrt(3);

	p1 = { 0, 0, 0 };
	p2 = glm::vec3(x, x, x);
	cord_ = spline;
	cord_->editPoint(0, p1);
	cord_->editPoint(1, p2);

	cord_->translate(object_->translation());
	velocity_ = { 0, 0, 0 };
	rest_length_ = 3;
	mass_ = 1;
	k_ = 100;
}

PendulumSpring::~PendulumSpring()
{
}

void PendulumSpring::update(float dt) {
	// Calc midpoint
	float length1 = glm::length(p2 - p1);
	glm::vec3 spring_force1 = k_ * (length1 - rest_length_) * (p2 - p1);
	glm::vec3 velocity1 = velocity_ + (spring_force1 + gravity_) * dt / 2.f;
	glm::vec3 midpoint = p1 + velocity1 * dt / 2.f;

	// Use to calculate translation
	float length2 = glm::length(p2 - midpoint);
	glm::vec3 spring_force2 = k_ * (length2 - rest_length_) * (p2 - midpoint);
	velocity_ += (spring_force2 + gravity_) * dt;
	p1 += velocity_ * dt;
	object_->translate(velocity_ * dt);
	cord_->editPoint(0, p1);
}

// TODO: this is a horrible rendering pipeline. Unify shaders.
void PendulumSpring::render(Program* program) const {
	object_->render(program, GL_DYNAMIC_DRAW);
	cord_->render(program, GL_DYNAMIC_DRAW);
}