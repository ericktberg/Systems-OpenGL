#include "Digraph.h"
#include <stdlib.h>

Digraph::
Digraph() : RenderableObject(GL_LINES) {
	glm::vec3 p1 = { 45, 45, 5 };
	glm::vec3 p2 = { -45, -45, 5 };

	addNode(p1);
	for (int i = 0; i < 1000; i++) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 90.f - 45.f;
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 90.f - 45.f;
		glm::vec3 p = { r1, r2, 1 };
		addNode(p);
	}
	addNode(p2);

	linkNodes(7);
}

Digraph::
~Digraph() {
	for (int i = 0; i < nodes_.size(); i++) {
		delete nodes_.at(i);
	}
}

void Digraph::
addNode(glm::vec3 p) {
	nodes_.push_back(new DiNode(p));
	vertices_.push_back({ p, { 1.f, 0.f, 0.f, 1 }, { 0, 0, 0 } });
}

void Digraph::
linkNodes(float distance) {
	for (int i = 0; i < nodes_.size(); i++) {
		for (int j = i; j < nodes_.size(); j++) {
			DiNode* n1 = nodes_.at(i);
			DiNode* n2 = nodes_.at(j);
			float length = glm::length(n1->position() - n2->position());
			if (length < distance) {
				n1->link(n2);
				n2->link(n1);
				edges_.push_back({ i, j });
			}
		}
	}

	edges_.push_back({ 0, nodes_.size() -1  });
	DiNode* n1 = nodes_.at(0);
	DiNode* n2 = nodes_.at(nodes_.size() - 1);
	n1->link(n2);
	n2->link(n1);
}

glm::vec3 Digraph::
direction(int idx1, int idx2) {
	DiNode* n1 = nodes_.at(idx1);
	DiNode* n2 = nodes_.at(idx2);

	return n2->position() - n1->position();
}

std::vector<int> Digraph::
shortestPath(int start, int end) {


}

