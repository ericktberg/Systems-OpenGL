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
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		int stretch[4];
		int shear[4];
		int bend[8];
	};

	Plane* plane_;
	std::vector<Node*> nodes_;

	float kstretch_, kdamp_, kshear_, kbend_, rest_stretch_, rest_shear_, rest_bend_;
};
