#include "DiNode.h"

DiNode::
DiNode(glm::vec3 pos) : position_(pos) { }

DiNode::
~DiNode() {
}



void DiNode::
link(int next) {
	linked_.push_back(next);
}