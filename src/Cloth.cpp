#include "Cloth.h"
#include "Plane.h"
#include "Sphere.h"

Cloth::Cloth() {}
Cloth::~Cloth() {}

void Cloth::init(Plane* plane) {
	plane_ = plane;
	nodes_.reserve(plane_->size());
	
	for (int i = 0; i < plane_->size(); i++){
		Node* node = new Node;
		std::vector<int> n = plane_->neighbors(i);
		node->position = plane_->point(i) + plane_->position();
		node->velocity = { 0, 0, 0 };
		node->acceleration = { 0, 0, -2 };
		int j = 0;
		while(j < 4) {
			node->stretch[j] = n[j];
			j++;
		}
		while (j < 8) {
			node->shear[j - 4] = n[j];
			j++;
		}
		while (j < 16) {
			node->bend[j - 8] = n[j];
			j++;
		}

		nodes_.push_back(node);
	}

	kstretch_ = 1000;
	kshear_ = 800;
	kbend_ = 50;
	kdamp_ = 100;
	rest_stretch_ = .1;
	rest_shear_ = .14141414;
	rest_bend_ = .22;
}	

void Cloth::update(float dt) {

}

glm::vec3 Cloth::euler_velocity(float dt, Node* node) {
	glm::vec3 stretch_force, shear_force, damp_force, bend_force;
	for (int j = 0; j < 4; j++) {
		if (node->stretch[j] >= 0) {
			glm::vec3 spring_stretch = nodes_.at(node->stretch[j])->position - node->position;
			glm::vec3 damp_stretch = nodes_.at(node->stretch[j])->velocity - node->velocity;
			float length = glm::length(spring_stretch);
			if (length - rest_stretch_) {
				stretch_force += kstretch_*(length - rest_stretch_) * spring_stretch / length;
			}

			damp_force += kdamp_ * (damp_stretch);
		}
		if (node->shear[j] >= 0) {
			glm::vec3 shear_stretch = nodes_.at(node->shear[j])->position - node->position;
			float length = glm::length(shear_stretch);
			if (length - rest_shear_ > 0){
				shear_force += kshear_ * (length - rest_shear_) * shear_stretch / length;
			}
		}
	}
	for (int j = 0; j < 8; j++) {
		if (node->bend[j] >= 0) {
			glm::vec3 bend_stretch = nodes_.at(node->bend[j])->position - node->position;
			float length = glm::length(bend_stretch);
			bend_force += kbend_ * (length - rest_bend_) * (length - rest_bend_) * bend_stretch / length;
		}
	}
	return (damp_force + stretch_force + shear_force + gravity_) * dt;
}

// TODO general and more realistic collisions.
// TODO spatial structure for collisions?
// TODO better integration
// TODO make this not shitty as hell

// Update the cloth
//----------------------------------------------------------------------------
// Uses the topology setup outlined in http://www.cs.ubc.ca/~ascher/papers/ba.pdf
// Stiff stretch springs on edge lengths
// Less stiff shear springs on diagonals
// Weak, non-linear bend springs on outer ring

void Cloth::update(float dt, const Sphere& sphere) {
	/**********************************
	* calc midpoints
	***********************************/
	//for (int i = 0; i < nodes_.size(); i++) {
	//	Node* node = nodes_.at(i);
	//	node->last_vel = node->velocity;
	//	node->velocity += euler_velocity(dt / 2, node);
	//}
	//for (int i = 0; i < nodes_.size(); i++) {
	//	Node* node = nodes_.at(i);
	//	node->last_pos = node->position;
	//	node->position += node->velocity * dt * .5f;
	//}
	/**********************************
	* calc result points
	***********************************/
	for (int i = 0; i < nodes_.size(); i++) {
		Node* node = nodes_.at(i);

		node->velocity += /*node->last_vel +*/ euler_velocity(dt, node);

		float t = sphere.collisionPoint(node->velocity, node->position/*last_pos*/);
		glm::vec3 point = node->velocity * t + node->position/*last_pos*/;
		glm::vec3 n = glm::normalize(point - sphere.center());
		if (t >= 0 && t < dt) {
			node->position = point;
			node->velocity += -n * glm::dot(node->velocity, n) * 2.f;
		}
	}
	for (int i = 0; i < nodes_.size(); i++) {
		Node* node = nodes_.at(i);

		node->position += /*node->last_pos +*/ node->velocity*dt;
		plane_->movePoint(i, node->velocity*dt);
	}
}

void Cloth::render() const {
	plane_->renderEdges(GL_DYNAMIC_DRAW);
}