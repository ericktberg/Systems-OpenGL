#pragma once

#include <glm/glm.hpp>

#include "RenderableObject.h"

#define PI 3.14159

class Camera {
public:
	Camera(float, float);
	~Camera();

	float rotV() const { return rot_v_; }
	float rotU() const { return rot_u_; }
	float dolly() const { return dolly_;  }

	void rotV(float dv) { rot_v_ += dv; }
	void rotU(float du) { rot_u_ += du; }
	void dollyInc(float inc) { dolly_ += inc; }

	glm::mat4 matrix() const { return proj_ * view_; }
	void renderEdgesFrom(const RenderableObject& object);
	void renderFacesFrom(const RenderableObject& object);
	void renderPointsFrom(const RenderableObject& object);
	void setCamera();



	float rot_v_, rot_u_, dolly_;

	glm::mat4 proj_;
	glm::mat4 view_;
};