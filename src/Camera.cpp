#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Program.h"

Camera::Camera(float window_width, float window_height) {
	proj_ = glm::perspective(glm::radians(45.0f), window_width / window_height, 1.0f, 100.0f);
	rot_v_ = 45;
	rot_u_ = 45;
	dolly_ = 14.14;
}

Camera::~Camera() {}

void Camera::setCamera() {
	glm::vec3 camera_pos = {
		cos(rot_v_ * PI / 180) * sin(rot_u_ * PI / 180) * dolly_,
		sin(rot_v_ * PI / 180) * sin(rot_u_ * PI / 180) * dolly_,
		dolly_ * cos(rot_u_ * PI / 180)
	};
	view_ = glm::lookAt(
		camera_pos,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f)
	);
}

void Camera::renderEdgesFrom(const RenderableObject& object) {
	Program* program = object.program();
	program->bind();
	program->setUniformMatrix4fv("camera", glm::value_ptr(matrix()));
	object.renderEdges(GL_DYNAMIC_DRAW);
}
void Camera::renderPointsFrom(const RenderableObject& object) {
	Program* program = object.program();
	program->bind();
	program->setUniformMatrix4fv("camera", glm::value_ptr(matrix()));
	object.renderPoints(GL_DYNAMIC_DRAW);
}
void Camera::renderFacesFrom(const RenderableObject& object) {
	Program* program = object.program();
	program->bind();
	program->setUniformMatrix4fv("camera", glm::value_ptr(matrix()));
	object.renderFaces(GL_DYNAMIC_DRAW);
}

void Camera::renderSystem(const DynamicObject& object) {
	object.render();
}