#pragma once

#include "Digraph.h"
#include "Sphere.h"
#include "Scene.h"
#include "glm/glm.hpp"

class Agent {
public:
	Agent();
	~Agent();

	void updatePosition(float dt);
	void calcRoute(const Scene& scene);

private:
	Sphere* body_;

	glm::vec3 position_;
	glm::vec3 current_direction_;

	Digraph prob_graph_;


};