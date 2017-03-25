#pragma once

#include <vector>
#include "glm/glm.hpp"
class DiNode {
public:
	DiNode(glm::vec3 pos);
	~DiNode();

	float from_start = -1;
	int previous = -1;
	bool visited = false;

	int size() const { return linked_.size(); }

	void link(int next);
	glm::vec3 position() const { return position_; }

	int at(int i) const { return linked_.at(i); }

private:
	std::vector<int> linked_;
	 
	glm::vec3 position_;

};