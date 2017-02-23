#pragma once
#include "Plane.h"
#include "Sphere.h"
#include "DynamicObject.h"

#include <vector>



class Cloth : public DynamicObject {
public:
	Cloth(Plane*);
	~Cloth();
	void render(Program*) const;
	void update(float dt);
	void update(float dt, const Sphere&);
	glm::vec3 calcNormals(Node*);

	glm::vec3 calcNormal(Node* p1, Node* p2, Node* p3);

private:

	//----------------------------------------------------------------------------
	
	glm::vec3 euler_velocity(float dt, Node*);
	glm::vec3 drag(float dt, Node*);

	//----------------------------------------------------------------------------
	// Cloth geometry
	Plane* plane_;

	//----------------------------------------------------------------------------
	// Physical spring variables
	float kstretch_, kdamp_, kshear_, kbend_, rest_stretch_, rest_shear_, rest_bend_90_, rest_bend_45_;
};
