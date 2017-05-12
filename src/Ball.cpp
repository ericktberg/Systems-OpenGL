#include "Ball.h"
#include <iostream>
#include <glm/gtx/norm.hpp>
#include "Block.h"
Ball::
Ball(float x, float y, float radius) : Circle(x, y, radius) {
	acceleration_ = { 0, -20, 0};
	velocity_ = { 0, 0, 0 };
}

void Ball::
update(float dt, Scene* scene) {
	if (glm::l1Norm(velocity_) > 50) {
		velocity_ *= .95f;
	}

	
	Payload p = scene->objectCollision(this, velocity_ * dt, position_);
	velocity_ += acceleration_*dt;

	if (p.t != -1) {
		scene->objectAt(p.id)->interact();

 		velocity_ = velocity_ - 2 * (glm::dot(velocity_, p.normal))*p.normal + p.velocity;
	}
	position_ += dt * velocity_;

	translate(position_.x, position_.y, 0);
}

void Ball::
start(bool status) { 
	if (!started_) {
		started_ = status;
		velocity_ = { -10 * status, -10 * status, 0 };
	}
}