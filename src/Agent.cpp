#include "Agent.h"

Agent::
Agent(Sphere* sphere, Digraph* digraph) : 
		body_(sphere), 
		prob_graph_(digraph), 
		path_(digraph->path_) 
{
	point = path_.size() - 1;
	position_ = { 45, 45, 1 };

	changeDirection();
}


Agent::
~Agent() {}


void Agent::
changeDirection() {
	if (point >= 0) {
		current_direction_ = glm::normalize(
			prob_graph_->posAt(path_.at(point))
			- position_
			);
	}
}

void Agent::
updatePosition(float dt) {
	if (point >= 0) {
		glm::vec3 next_pos = prob_graph_->posAt(path_.at(point));

		if (glm::length(position_ - next_pos) < 1) {
			point--;
			changeDirection();
		}

		body_->translate(current_direction_ * dt * 6.f);
		position_ += current_direction_ * dt * 6.f;
	}
}