#pragma once

#include <glm/glm.hpp>



class Camera {
public:
	Camera(float, float);
	~Camera();

	glm::mat4 matrix() const { return proj_; }
private:

	glm::mat4 proj_;
};