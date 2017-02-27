/****************************************************************************************
* DynamicObject is a purely virtual class that defines physics equations that 
* it's derived classes may use to update their Renderable aspects.
*****************************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include "Program.h"

#include <vector>

class DynamicObject {
public:
	DynamicObject(glm::vec3 gravity = { 0, 0, -9.8 });
	~DynamicObject();
	virtual void update(float dt) = 0;
	virtual void render(Program*) const = 0;

protected:
	struct Node {
		// Integration elements
		glm::vec3 position;
		glm::vec3 last_pos;
		glm::vec3 velocity;
		glm::vec3 last_vel;
		glm::vec3 acceleration;

		glm::vec3 normal;
		// Neighbors
		int stretch[4];
		int shear[4];
		int bend[8];

		// Status variables
		bool pinned = false;

	};
	
	glm::vec3 euler_integrate();

	glm::vec3 midpoint_integrate();

	std::vector<Node*> nodes_;

	virtual glm::vec3 bounce(
		float COD,
		glm::vec3 velocity,
		glm::vec3 normal
		) const;

	virtual glm::vec3 friction(
		float k_static,
		float k_dynamic,
		glm::vec3 velocity,
		glm::vec3 normal
		) const;

	virtual glm::vec3 airResistTriangle(
		Node* p1,
		Node* p2,
		Node* p3) const;

	virtual glm::vec3 springForce(
		float k,
		float rest_length,
		Node* node1,
		Node* node2,
		bool compression = true,
		int degree = 1
		) const;

	virtual glm::vec3 dampen(
		float k,
		Node* node1,
		Node* node2
		) const;

	//----------------------------------------------------------------------------
	// Constants
	glm::vec3 gravity_;
};