#pragma once
#include "Plane.h"
#include "Sphere.h"
#include "DynamicObject.h"

#include <vector>



class Cloth : public DynamicObject {
public:
	Cloth();
	~Cloth();
	void init(Plane* plane);
	void render() const;
	void update(float dt);
	void update(float dt, const Sphere& sphere);
private:
	struct Node {
		// Integration elements
		glm::vec3 position;
		glm::vec3 last_pos;
		glm::vec3 velocity;
		glm::vec3 last_vel;
		glm::vec3 acceleration;
		// Neighbors
		int stretch[4];
		int shear[4];
		int bend[8];

		// Status variables
		bool pinned;

	};

	glm::vec3 euler_velocity(float dt, Node* node);

	Plane* plane_;
	std::vector<Node*> nodes_;

	float kstretch_, kdamp_, kshear_, kbend_, rest_stretch_, rest_shear_, rest_bend_;
};
