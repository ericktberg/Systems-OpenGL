
//	glm::vec3 cord_vector = cord_->vector();
//	glm::vec3 spring_force = k_*(length_ - rest_length_) * cord_vector;
//	velocity_ += (spring_force + gravity_) * dt;
//	cord_->movePoint(0, velocity_ * dt);
//	object_->translate(velocity_ * dt);
//	length_ = glm::length(cord_vector);


#include "PendulumSpring.h"
#include <iostream>

PendulumSpring::PendulumSpring() : DynamicObject()
{
}

PendulumSpring::~PendulumSpring()
{
}

void PendulumSpring::update(float dt) {
	glm::vec3 spring_force = k_*(length_ - rest_length_) * cord_->vector();
	velocity_ += (spring_force / mass_ + gravity_) * dt;
	p1 += velocity_ * dt;
	object_->translate(velocity_ * dt);
	cord_->movePoint(0, velocity_ * dt);
	length_ = glm::length(cord_->vector());
}

void PendulumSpring::init(RenderableObject* object) {
	object_ = object;
	float x = sqrt(3);
	p1 = { 0, 0, 0 };
	p2 = glm::vec3(x, x, x);
	cord_ = new Spline({ 0, 0, 0 }, { x, x, x });
	cord_->translate(object_->translation());
	velocity_ = { 0, 0, 0 };
	rest_length_ = 3;
	length_ = 3;
	mass_ = 1;
	k_ = 100;
}

void PendulumSpring::render() const {
	object_->renderEdges(GL_DYNAMIC_DRAW);
	cord_->renderEdges(GL_DYNAMIC_DRAW);
}