#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>
Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::
render(const Camera& camera) const {
	for (int i = 0; i < objects_.size(); i++){
		RenderableObject* object = objects_.at(i);
		Program* program = programs_.at(program_map_.at(i));
		program->bind();
		program->setUniformMatrix4fv("camera", glm::value_ptr(camera.matrix()));
		object->render(program, GL_DYNAMIC_DRAW);
	}
}

void Scene::
assignShader(int object_idx, int program_idx) {
	objects_.at(object_idx)->vaoInit(programs_.at(program_idx));
	program_map_[object_idx] = program_idx;
}

bool Scene::
intersect(glm::vec3 point) const {
	for (int i = 0; i < objects_.size(); i++) {
		if (objects_.at(i)->within(point)) {
			return true;
		}
	}
	return false;
}

int Scene::
addShader(Program* program) {
	programs_.push_back(program);
	return programs_.size() - 1;
}

int Scene::
addObject(RenderableObject* object) {
	object->setIndex(objects_.size());
	objects_.push_back(object); 
	return object->index(); 
}

Payload Scene:: 
objectCollision(const RenderableObject* source,
	const glm::vec3& velocity,
	const glm::vec3& origin)
const {
	Payload closest = { -1, -1, { 0, 0, 0 } };
	for (int i = 0; i < objects_.size(); i++) {
		if (i != source->index()) {
			if (i == 0) {
  				auto breakpoint = i;
			}
			Payload p = objects_.at(i)->rayCollision(velocity, origin);
			bool smaller = p.t < closest.t;
			bool first = closest.t < 0;

			if (p.t != -1) {
				closest = (smaller || first) ? p : closest;
				closest.id = i;
				if (i == 0) {
					auto breakpoint = i;
				}
			}
			
		}
	}

	return closest;
}