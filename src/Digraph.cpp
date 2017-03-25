#include "Digraph.h"
#include <stdlib.h>

Digraph::
Digraph(const Scene& scene) : RenderableObject(GL_LINES) {
	glm::vec3 p1 = { 45, 45, 1 };
	glm::vec3 p2 = { -45, -45, 1 };

	addNode(p1);
	for (int i = 0; i < 1000; i++) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 90.f - 45.f;
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)* 90.f - 45.f;
		glm::vec3 p = { r1, r2, 1 };
		if (!scene.intersect(p)){
			addNode(p);
		}
	}
	addNode(p2);

	linkNodes(7);
	path_ = shortestPath(0, nodes_.size() - 1);
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
				n1->link(j);
				n2->link(i);
				edges_.push_back({ i, j });
			}
		}
	}
}

glm::vec3 Digraph::
direction(int idx1, int idx2) {
	DiNode* n1 = nodes_.at(idx1);
	DiNode* n2 = nodes_.at(idx2);

	return n2->position() - n1->position();
}

std::vector<int> Digraph::
shortestPath(int start, int end) {
	vertices_.at(start).color = { 0.f, 1.f, 1.f, 1 };
	vertices_.at(end).color = { 0.f, 1.f, 1.f, 1 };

	nodes_.at(0)->from_start = 0;
	visit(0);

	std::vector<int> path;
	int previous = end;
	while (previous != start) {
		vertices_.at(previous).color = { 1.f, 0.f, 0.f, 1 };
		path.push_back(previous);
		DiNode* node = nodes_.at(previous);
		previous = node->previous;
	}
	return path;
}

void Digraph::
visit(int idx) {
	DiNode* n1 = nodes_.at(idx);
	n1->visited = true;

	for (int j = 0; j < n1->size(); j++) {
		int next = n1->at(j);

		float distance = glm::length(direction(idx, next));
		DiNode* n2 = nodes_.at(next);

		if (!n2->visited){

			if (n2->from_start == -1 || n2->from_start > n1->from_start + distance) {
				n2->from_start = n1->from_start + distance;
				n2->previous = idx;
				vertices_.at(next).color = { 0.f, 0.f, 1.f, .3 };
			}
		}
	}

	for (int j = 0; j < n1->size(); j++) {
		int next = n1->at(j);
		DiNode* n2 = nodes_.at(next);

		if (!n2->visited){
			visit(next);
		}
	}
}