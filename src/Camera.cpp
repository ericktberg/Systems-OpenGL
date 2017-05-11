#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Program.h"

Camera::Camera(float window_width, float window_height) {
	proj_ = glm::ortho(-40.f, 40.f, -20.f, 20.f);
}

Camera::~Camera() {}
