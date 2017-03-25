#pragma once

#include <vector>
#include "glm/glm.hpp"
class DiNode {
public:
	DiNode(glm::vec3 pos);
	~DiNode();

	void link(DiNode* next);
	glm::vec3 position() const { return position_; }

private:
	std::vector<DiNode*> linked_;

	glm::vec3 position_;
};