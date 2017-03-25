#pragma once

#include "glm/glm.hpp"
#include "DiNode.h"
#include "RenderableObject.h"
#include "Scene.h"

class Digraph : public RenderableObject {
public:
	Digraph(const Scene&);
	~Digraph();

	void addNode(glm::vec3 p);
	std::vector<int> shortestPath(int node1, int node2);
	void visit(int idx);
	glm::vec3 direction(int node1, int node2);

	void linkNodes(float distance);

	glm::vec3 posAt(int idx) const { return nodes_.at(idx)->position(); }

	std::vector<int> path_;


private:
	std::vector<DiNode*> nodes_;

};