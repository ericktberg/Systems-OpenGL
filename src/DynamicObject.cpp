#include "DynamicObject.h"

DynamicObject::DynamicObject(glm::vec3 gravity) : gravity_(gravity) {

}

DynamicObject::~DynamicObject() {

}


glm::vec3 DynamicObject::
bounce(
	float COD,
	glm::vec3 velocity,
	glm::vec3 normal
) const {
	return{ 0, 0, 0 };
}

glm::vec3 DynamicObject::
friction(
	float k_static,
	float k_dynamic,
	glm::vec3 velocity,
	glm::vec3 normal
) const {
	return{ 0, 0, 0 };
}

// Calculate air resistance of a triangle
//----------------------------------------------------------------------------
// TODO optimization
glm::vec3 DynamicObject::
airResistTriangle(
	Node* p1,
	Node* p2,
	Node* p3) const {
	glm::vec3 result;
	glm::vec3 velocity = (p3->velocity + p2->velocity + p3->velocity) / 3.f;
	if (glm::length(velocity) >= .001){
		float v = glm::length(velocity);
		glm::vec3 nstar = glm::cross((p2->position - p1->position), (p3->position, p1->position));
		result =  -.5f * .1f * v * (glm::dot(velocity, nstar)) / (glm::length(nstar)) * nstar;
		
	}
	else {
		result = { 0, 0, 0 };
	}
	return result;
}

// Force of spring
//----------------------------------------------------------------------------
// 
glm::vec3 DynamicObject::
springForce(
	float k,
	float rest_length,
	Node* node1,
	Node* node2,
	bool compression,
	int degree
) const {
	glm::vec3 vector = node2->position - node1->position;

	float length = glm::length(vector);
	float diff = length - rest_length;
	if (compression || diff > 0.01) {
		for (int i = 1; i < degree; i++) {
			diff *= diff;
		}
		return k * diff * vector / length;
	}
	else {
		return { 0, 0, 0 };
	}
}

// Dampen velocity between nodes
//----------------------------------------------------------------------------
//
glm::vec3 DynamicObject::
dampen(
	float k,
	Node* node1,
	Node* node2
) const {
	glm::vec3 vector = node2->velocity - node1->velocity;
	if (glm::length(vector) > .01) {
		return k * vector;
	}
	else {
		return{ 0, 0, 0 }; 
	}
}
