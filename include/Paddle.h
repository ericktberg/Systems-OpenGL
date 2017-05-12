

#pragma once
#include <algorithm>
#include "RenderableObject.h"
#include "Scene.h"
#include "Block.h"

//#define DEBUG_PADDLE 1
class Paddle : public RenderableObject{
public:
	Paddle(float x, float y, float width);
	Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin);
	virtual void translate(float x, float y, float z);
	void update(float dt);
	void pullBack(float distance);
	void release();
	Scene* scene;
	int grace = 0;

private:
	

	struct Node {
		// Integration elements
		glm::vec3 position;
		glm::vec3 last_pos;
		glm::vec3 velocity;
		glm::vec3 last_vel;
		glm::vec3 acceleration;

		bool pinned = false;
	};

	glm::vec3 springForce(Node* node1, Node* node2);
	glm::vec3 dampen(Node* node1, Node* node2);
	std::vector<Node> nodes_;
	float k = 2000;
	float damp = 10;
	float rest_length = 0;

	bool moved_ = false;
	glm::vec3 previous_pos_;
};


inline bool validate(float t, float u) {
	return t > 0 && t < 1 && u > 0 && u < 1;
}