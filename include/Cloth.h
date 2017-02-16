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
	struct Particle {
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		int neighbors[4];
	};

	Plane* plane_;
	std::vector<Particle*> nodes_;

	float ks_, kd_, rest_length_;
};
