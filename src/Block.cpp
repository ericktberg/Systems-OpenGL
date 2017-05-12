#include "Block.h"

Block::
Block(float x, float y, float height, float width, int state, glm::vec4 color) : Rectangle(x, y, height, width, color) {
	state_ = state;
}

Block::Block(float x, float y, float height, float width, glm::vec4 color) : Block(x, y, height, width, SOLID, color) {

}

Block::
Block(float x, float y, float height, float width, int state) : Block(x,y,height,width,state,WHITE) {
}


Block::
Block(float x, float y, float height, float width) : Block(x, y, height, width, SOLID) {

}

Block::
~Block() {}

void Block::
renderEdges(Program* program, GLenum usage) const {
	prepareRender(program);

	if (state_ != POPPED) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, edges_.size() * sizeof(Edge), edges_.data(), usage);
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);

		glDrawElements(GL_LINES, edges_.size() * 2, GL_UNSIGNED_INT, (void*)0);
	}
}

Payload Block::
rayCollision(const glm::vec3& velocity, const glm::vec3& origin) {
	
	Payload payload = { -1, -1, { 0, 0, 0 } };
	if (state_ == SOLID) {
		payload = rectangleIntersection(velocity, origin);
	}
	return payload;
}