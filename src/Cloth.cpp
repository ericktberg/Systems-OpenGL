#include "Cloth.h"
#include "Plane.h"
#include "Sphere.h"

Cloth::Cloth() {}
Cloth::~Cloth() {}

void Cloth::init(Plane* plane) {
	plane_ = plane;
	nodes_.reserve(plane_->size());
	
	for (int i = 0; i < plane_->size(); i++){
		Particle* node = new Particle;
		std::vector<int> n = plane_->neighbors(i);
		node->position = plane_->point(i) + plane_->position();
		node->velocity = { 0, 0, 0 };
		node->acceleration = { 0, 0, -2 };
		for (int j = 0; j < 4; j++) {
			node->neighbors[j] = n[j];
		}

		nodes_.push_back(node);
	}

	ks_ = 1000;
	kd_ = 10;
	rest_length_ = .1;
}	

void Cloth::update(float dt) {

}


void Cloth::update(float dt, const Sphere& sphere) {

	for (int i = 0; i < nodes_.size(); i++) {
		Particle* node = nodes_.at(i);

		glm::vec3 spring_force;
		glm::vec3 damp_force;
		for (int i = 0; i < 4; i++) {
			if (node->neighbors[i] >= 0) {
				glm::vec3 spring = nodes_.at(node->neighbors[i])->position - node->position;
				glm::vec3 damp = nodes_.at(node->neighbors[i])->velocity - node->velocity;

				spring_force += ks_*(glm::length(spring) - rest_length_) * glm::normalize(spring);
				damp_force += kd_ * (damp);
			}
		}

		node->velocity += (damp_force + spring_force + gravity_) * dt;
	}
	for (int i = 0; i < nodes_.size(); i++) {
		Particle* node = nodes_.at(i);

		float t = sphere.collisionPoint(node->velocity, node->position);
		glm::vec3 point = node->velocity * t + node->position;
		glm::vec3 n = glm::normalize(point - sphere.center());

		if (abs(t) < dt) {
			node->velocity += n * glm::dot(node->velocity, n);
		}
		node->position += node->velocity*dt;
		plane_->movePoint(i, node->velocity*dt);
	}
}

void Cloth::render() const {
	plane_->renderEdges(GL_DYNAMIC_DRAW);
}