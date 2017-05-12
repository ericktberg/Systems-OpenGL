#include "Paddle.h"

Paddle::
Paddle(float x, float y, float width) : RenderableObject(GL_LINES) {
	int steps = 10;
	float stepsize = width / steps;
	rest_length = stepsize;
	int i = 0;
	for (float step = -width / 2; step <= width / 2; step += stepsize, i++) {
		std::cout << step << std::endl;
		vertices_.push_back({ { step, 0, 0 }, { 0, 1, 0, 1 } });
		Node node;
		node.position = { step, 0, 0 };
		if (!i || i == steps) {
			node.pinned = true;
		}
		nodes_.push_back(node);

		if (i)
			edges_.push_back({ i - 1, i });
	}

	translate(x, y, 0);

	cor_ = .8;
}

/* Paddle moves frequently. Robustly handle intersections between moving targets.
 * Intersect at current state, prior state, and velocity vectors up to it

			 v current
		   _____
		  /    /  <-- imaginary line segment connecting the two
		 /____/
	
			 ^ previous

 * Determine ball-paddle orientation (direction of normal) based on prior state.
 */
Payload Paddle::
rayCollision(const glm::vec3& velocity, const glm::vec3& origin) {

	#ifdef DEBUG_PADDLE
		glm::vec4 color = sign > 0 ? MAGENTA : CYAN;
		scene->assignShader(scene->addObject(new Block(v1.x, v1.y, .5, .5, POPPING, GREEN)), 0);
		scene->assignShader(scene->addObject(new Block(v2.x, v2.y, .5, .5, POPPING, BLUE)), 0);
		scene->assignShader(scene->addObject(new Block(test.x, test.y, .2, .2, POPPING, color)), 0);
	#endif
	
	float t_prior = -1, u_prior = -1, t_post = -1, u_post = -1;
	float t_v1 = -1, u_v1 = -1, t_v2 = -1, u_v2 = -1;
	glm::vec3 norm_prior, norm_post, norm_v1, norm_v2;

	Payload payload = { -1, -1 };
	if (grace <= 0) {
		for (int i = 0; i < edges_.size(); i++) {
			/* ray-line intersection for each edge */
			Edge edge = edges_.at(i);
			glm::vec3 normal;

			lineIntersection(velocity, origin,
				vertices_.at(edge.p1).position + previous_pos_,
				vertices_.at(edge.p2).position + previous_pos_,
				&t_prior, &u_prior, &normal);

			lineIntersection(velocity, origin,
				vertices_.at(edge.p1).position + position_,
				vertices_.at(edge.p2).position + position_,
				&t_post, &u_post, &norm_post);

			lineIntersection(velocity, origin,
				vertices_.at(edge.p1).position + previous_pos_,
				vertices_.at(edge.p1).position + position_,
				&t_v1, &u_v1, &norm_v1);

			lineIntersection(velocity, origin,
				vertices_.at(edge.p2).position + previous_pos_,
				vertices_.at(edge.p2).position + position_,
				&t_v2, &u_v2, &norm_v2);

			float t = -1, u = -1;
			if (validate(t_prior, u_prior)) {
				t = t_prior;
				u = u_prior;
				//normal = norm_prior;
			}
			if (t == -1 || (validate(t_post, u_post) && t_post < t)) {
				t = t_post;
				u = u_post;
				//normal = norm_post;
			}
			if (t == -1 || (validate(t_v1, u_v1) && t_v1 < t)) {
				t = t_v1;
				u = u_v1;
				//normal = norm_v1;
			}
			if (t == -1 || (validate(t_v2, u_v2) && t_v2 < t)) {
				t = t_v2;
				u = u_v2;
				//normal = norm_v2;
			}

			if (validate(t, u)) {
				payload.t = (t < payload.t || payload.t < 0) ? t : payload.t;
				payload.velocity = velocity_;
				payload.normal = normal;
				payload.t = t;
				payload.id = index();
				grace = 10;

			}
		}
	}
	return payload;
}


void Paddle::
translate(float x, float y, float z) {
	previous_pos_ = position_;
	velocity_ = glm::vec3(x - position_.x, y - position_.y, z - position_.z) / .03f;

	position_ = { x, y, z };
	translation_ = {
		{ 1, 0, 0, 0 },
		{ 0, -1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 } };
}

void Paddle::
update(float dt) {
	for (int i = 1, len = nodes_.size() - 1; i < len; i++) {
		if (!nodes_.at(i).pinned) {
			nodes_.at(i).velocity += (springForce(&nodes_.at(i), &nodes_.at(i - 1))
				+ springForce(&nodes_.at(i), &nodes_.at(i + 1))
				+ dampen(&nodes_.at(i), &nodes_.at(i - 1))
				+ dampen(&nodes_.at(i), &nodes_.at(i + 1)))
				* dt;
		}
	}
	for (int i = 1, len = nodes_.size() - 1; i < len; i++) {
		if (!nodes_.at(i).pinned) {
			if (glm::length(nodes_.at(i).velocity) < .05) {
				nodes_.at(i).velocity = { 0, 0, 0 };
				nodes_.at(i).position.y = 0;
				vertices_.at(i).position.y = 0;
			}
			else {
				vertices_.at(i).position += nodes_.at(i).velocity * dt;
				nodes_.at(i).position += nodes_.at(i).velocity * dt;
			}
		}
	}
}

void Paddle::
pullBack(float distance) {
	int i = nodes_.size() / 2;
	nodes_.at(i).pinned = true;
	for (int j = -5; j < 6; j++) {
		vertices_.at(i + j).position.y = -sqrt(distance * (5 - abs(j)));
		nodes_.at(i + j).position.y = -sqrt(distance * (5 - abs(j)));
		nodes_.at(i + j).pinned = true;
	}
}

void Paddle::
release() {
	for (int j = -4; j < 5; j++) {
		nodes_.at(nodes_.size() / 2 + j).pinned = false;
	}
}

glm::vec3 Paddle::
springForce(Node* node1, Node* node2) {
	glm::vec3 vector = node2->position - node1->position;

	float length = glm::length(vector);
	float diff = length - rest_length;
	if (diff > 0.01) {
		return k * diff * vector / length;
	}
	else {
		return{ 0, 0, 0 };
	}
}

glm::vec3 Paddle::
dampen(Node* node1,Node* node2)  {
	glm::vec3 vector = node2->velocity - node1->velocity;
	if (glm::length(vector) > .01) {
		return damp * vector;
	}
	else {
		return{ 0, 0, 0 };
	}
}
