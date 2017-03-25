#pragma once

#include "Digraph.h"
#include "Sphere.h"
#include "Scene.h"
#include "glm/glm.hpp"

class Agent {
public:
	Agent(Sphere* sphere, Digraph* digraph);
	~Agent();

	void updatePosition(float dt);

private:
	void changeDirection();
	
	Sphere* body_;



	glm::vec3 position_;
	glm::vec3 current_direction_;

	Digraph* prob_graph_;
	std::vector<int> path_;

	int point;


};