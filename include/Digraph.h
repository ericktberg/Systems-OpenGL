#pragma once

#include "glm/glm.hpp"
#include "DiNode.h"
#include "RenderableObject.h"

class Digraph : public RenderableObject {
public:
	Digraph();
	~Digraph();

	void addNode(glm::vec3 p);
	std::vector<int> shortestPath(int node1, int node2);
	glm::vec3 direction(int node1, int node2);

	void linkNodes(float distance);


private:
	std::vector<DiNode*> nodes_;

};