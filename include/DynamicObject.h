/****************************************************************************************
* DynamicObject is a purely virtual class that defines physics equations that 
* it's derived classes may use to update their Renderable aspects.
*****************************************************************************************/
#pragma once
#include <glm/glm.hpp>
class DynamicObject {
public:
	DynamicObject(glm::vec3 gravity = { 0, 0, -9.8 });
	~DynamicObject();
	virtual void update(float dt) = 0;
protected:
	glm::vec3 gravity_;
};