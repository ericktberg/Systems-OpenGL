#pragma once

#include "Camera.h"
#include "Program.h"
#include "RenderableObject.h"

#include <vector>
#include <map>

class Scene {
public:
	Scene();
	~Scene();

	void render(const Camera& camera) const;
	
	int addObject(RenderableObject* object) { objects_.push_back(object); return objects_.size() - 1; }
	int addShader(Program* program) { programs_.push_back(program); return programs_.size() - 1; }

	void assignShader(int object_idx, int program_idx);
	
	void translate(int object_idx, glm::vec3 displacement) { objects_.at(object_idx)->translate(displacement); }

private:
	std::map<int/*object*/, int/*program*/> program_map_;

	std::vector<Program*> programs_;
	std::vector<RenderableObject*> objects_;
};