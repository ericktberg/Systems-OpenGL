#pragma once

#include "Camera.h"
#include "Payload.h"
#include "Program.h"
#include "RenderableObject.h"

#include <vector>
#include <map>

class Scene {
public:
	Scene();
	~Scene();

	void render(const Camera& camera) const;

	int size() const { return objects_.size(); }

	RenderableObject* objectAt(int idx) const { return objects_.at(idx); }
	Program* programAt(int idx) const { return programs_.at(idx); }
	std::vector<RenderableObject*> objects() const { return objects_; }
	
	int addObject(RenderableObject* object);
	int addShader(Program* program);

	void assignShader(int object_idx, int program_idx);
	
	void translate(int object_idx, glm::vec3 displacement) { objects_.at(object_idx)->translate(displacement); }

	Payload objectCollision(const RenderableObject*, 
						const glm::vec3& velocity, 
						const glm::vec3& origin);
	bool intersect(glm::vec3 point) const;

private:
	std::map<int/*object*/, int/*program*/> program_map_;

	std::vector<Program*> programs_;
	std::vector<RenderableObject*> objects_;
};