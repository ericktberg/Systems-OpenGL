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
	Program* program() const { return plane_->program(); }
	glm::vec3 calcNormals(Node* node);

	glm::vec3 calcNormal(Node* node, Node* p1, Node* p2);

private:


	glm::vec3 euler_velocity(float dt, Node* node);
	glm::vec3 drag(float dt, Node* node);

	Plane* plane_;

	float kstretch_, kdamp_, kshear_, kbend_, rest_stretch_, rest_shear_, rest_bend_90_, rest_bend_45_;
};
