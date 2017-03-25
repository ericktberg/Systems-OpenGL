#include "DiNode.h"

DiNode::
DiNode(glm::vec3 pos) : position_(pos) { }

DiNode::
~DiNode() {
	for (int i = 0; i < linked_.size(); i++) {
		delete linked_.at(i);
	}
}



void DiNode::
link(DiNode* next) {
	linked_.push_back(next);
}