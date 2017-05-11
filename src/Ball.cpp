#include "Ball.h"
#include <iostream>
#include <glm/gtx/norm.hpp>
#include "Block.h"
Ball::
Ball(float x, float y, float radius) : Circle(x, y, radius) {
	acceleration_ = { 0, 0, 0 };
	velocity_ = { 0, 0, 0 };
}

void Ball::
update(float dt, Scene* scene) {
	if (glm::l1Norm(velocity_) > 50) {
		velocity_ *= .95f;
	}
	
	Payload p = scene->objectCollision(this, velocity_ * dt, position_);
	
	if (p.t != -1) {
		std::cout << p;
		scene->objectAt(p.id)->interact();

		glm::vec3 test = position_ + dt * velocity_;
		scene->assignShader(scene->addObject(new Block(test.x, test.y, .5, .5, POPPING)), 0);


 		velocity_ = velocity_ - 2 * (glm::dot(velocity_, p.normal))*p.normal;// +p.velocity;
	}
	position_ += dt * velocity_;

	translate(position_.x, position_.y, position_.z);
}

void Ball::
start(bool status) { 
	if (!started_) {
		started_ = status;
		velocity_ = { -10 * status, 10 * status, 0 };
	}
}