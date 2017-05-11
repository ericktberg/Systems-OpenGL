#pragma once

#include "Rectangle.h"

#define POPPED 1
#define SOLID 2
#define POPPING 3

class Block : public Rectangle {
public:
	Block(float x, float y, float height, float width, int state, glm::vec4 color);
	Block(float x, float y, float height, float width, int state);
	Block(float x, float y, float height, float width, glm::vec4 color);
	Block(float x, float y, float height, float width);
	~Block();

	void interact() { state_ = POPPED; }

	Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin) const;
	void renderEdges(Program* program, GLenum usage) const;
private:

	int state_ = SOLID;
};