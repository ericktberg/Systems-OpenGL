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