#include "Cloth.h"
#include "Plane.h"
#include "Sphere.h"

#include <vector>

Cloth::Cloth(Plane* plane) {
	plane_ = plane;
	nodes_.reserve(plane_->size());

	for (int i = 0; i < plane_->size(); i++){
		Node* node = new Node;
		std::vector<int> n = plane_->neighbors(i);
		node->position = plane_->point(i) + plane_->position();
		node->last_pos = node->position;
		node->velocity = { 0, 0, 0 };
		node->acceleration = { 0, 0, -2 };
		int j = 0;
		while (j < 8) {
			node->shear[j / 2] = n[j];
			j++;
			node->stretch[j / 2] = n[j];
			j++;
		}
		while (j < 16) {
			node->bend[j - 8] = n[j];
			j++;
		}

		nodes_.push_back(node);
		if (i < 21) {
			//node->pinned = true;
		}
	}

	kstretch_ = 2000;
	kshear_ = 2000;
	kbend_ = 50;
	kdamp_ = 100;
	rest_stretch_ = .1;
	rest_shear_ = sqrt(.1 * .1 * 2);
	rest_bend_90_ = .2;
	rest_bend_45_ = sqrt(.2 * .2 * 2);
}
Cloth::~Cloth() {}

glm::vec3 Cloth::euler_velocity(float dt, Node* node) {
	glm::vec3 stretch_force, shear_force, damp_force, bend_force;
	for (int j = 0; j < 4; j++) {
		if (node->stretch[j] >= 0) {
			stretch_force += springForce(kstretch_, rest_stretch_, node, nodes_.at(node->stretch[j]), true, 1);
			damp_force += dampen(kdamp_, node, nodes_.at(node->stretch[j]));
		}
		if (node->shear[j] >= 0) {
			shear_force += springForce(kshear_, rest_shear_, node, nodes_.at(node->shear[j]), true, 1);
		}
	}
	for (int j = 0; j < 8; j++) {
		if (node->bend[j] >= 0) {
			float rest = (j > 3 ? rest_bend_45_ : rest_bend_90_);
			bend_force += springForce(kbend_, rest, node, nodes_.at(node->bend[j]), true, 2);
		}
	}
	return (damp_force + stretch_force + shear_force + gravity_) * dt;
}

glm::vec3 Cloth::
drag(float dt, Node* node) {
	glm::vec3 drag_force;
	
	for (int i = 0; i < 4; i++) {
		bool x = node->shear[i] >= 0 && node->stretch[i] >= 0;
		bool y = node->shear[(i + 1) < 4 ? i + 1 : i] >= 0 && node->stretch[i] >= 0;

		if (x) {
			Node* p1 = nodes_.at(node->shear[i]);
			Node* p2 = nodes_.at(node->stretch[i]);
			drag_force += airResistTriangle(node, p1, p2);
			
		}
		if (y) {
			Node* p2 = nodes_.at(node->stretch[i]);
			Node* p3 = nodes_.at(node->shear[(i + 1) < 4 ? i + 1 : i]);
			drag_force += airResistTriangle(node, p2, p3);		
		}
	}

	return drag_force;
}

//----------------------------------------------------------------------------
// Naively calculate normals through equal face weighting
glm::vec3 Cloth::
calcNormals(Node* node) {

	glm::vec3 n;
	for (int i = 0; i < 4; i++) {
		bool x = node->shear[i] >= 0 && node->stretch[i] >= 0;
		bool y = node->shear[(i + 1) < 4 ? i + 1 : 0] >= 0 && node->stretch[i] >= 0;

		if (x) {
			Node* p1 = nodes_.at(node->shear[i]);
			Node* p2 = nodes_.at(node->stretch[i]);
			glm::vec3 s = glm::normalize(plane_->calcNormal(node->position, p1->position, p2->position));
			n += s;

		}
		if (y) {
			Node* p2 = nodes_.at(node->stretch[i]);
			Node* p3 = nodes_.at(node->shear[(i + 1) < 4 ? i + 1 : 0]);
			glm::vec3 s = glm::normalize(plane_->calcNormal(node->position, p2->position, p3->position));
			n += s;
		}
	}
	return glm::normalize(n);
}

// TODO general and more realistic collisions.
// TODO spatial structure for collisions?
// TODO better integration (RK4?) (IMEX?)
// TODO bend force is broken

// Update the cloth
//----------------------------------------------------------------------------
// Uses the topology setup outlined in http://www.cs.ubc.ca/~ascher/papers/ba.pdf
// Stiff stretch springs on edge lengths
// Less stiff shear springs on diagonals
// Weak, non-linear bend springs on outer ring

void Cloth::
update(float det, const std::vector<RenderableObject*>& objects, int this_idx) {
	float dt = .001;
	int step = det / dt;
	
	
	for(int i = 0; i < step; i++)  {
		
		/**********************************
		* calc midpoints
		***********************************/
		for (int i = 0; i < nodes_.size(); i++) {
			Node* node = nodes_.at(i);
			if (!node->pinned) {
				node->last_vel = node->velocity;
				glm::vec3 springs = euler_velocity(dt * .5, node);
				node->velocity += drag(dt * .5, node) + springs;
			}
		}
		for (int i = 0; i < nodes_.size(); i++) {
				Node* node = nodes_.at(i);
			if (!node->pinned) {
				node->last_pos = node->position;
				node->position += node->velocity * dt * .5f;
			}
		}
		/**********************************
		* calc result points
		***********************************/
		for (int i = 0; i < nodes_.size(); i++) {
			Node* node = nodes_.at(i);
			if (!node->pinned) {

				glm::vec3 springs = euler_velocity(dt, node);
				node->velocity = node->last_vel + drag(dt, node) + springs;

				// find collision
				float t = dt + 1, test;
				RenderableObject* intersected;
				for (int j = 0; j < objects.size(); j++) {
					if (j != this_idx) {
						test = objects.at(j)->collisionPoint(node->velocity, node->last_pos);
						if (test >= 0 && test < t) {
							t = test;
							intersected = objects.at(j);
						}
					}
				}


				if (t >= 0 && t < dt) {
					//node->last_pos = n * 1.001f + sphere.center();
					glm::vec3 point = node->velocity * t + node->/*position*/last_pos;
					glm::vec3 n = intersected->normal(point);
					glm::vec3 vel = 2.f * -n * glm::dot(node->velocity, n);
					node->velocity += vel;
				}
				else if (node->velocity.z * dt + node->last_pos.z < 0) {
					node->velocity.z = .8 * abs(node->velocity.z);
				}
			}
		}
		for (int i = 0; i < nodes_.size(); i++) {

			Node* node = nodes_.at(i);
			if (!node->pinned) {

				node->position = node->last_pos + node->velocity*dt;
				// TODO: rework to avoid subtraction here.
				plane_->editPoint(i, node->position - plane_->position());
			}
		}
	}
	// update normals
	// TODO: move this into plane
	for (int i = 0; i < nodes_.size(); i++) {
		plane_->updateNormal(i, calcNormals(nodes_.at(i)));
		plane_->calcNormals();
	}
}

void Cloth::render(Program* program) const {
	plane_->render(program, GL_DYNAMIC_DRAW);
}