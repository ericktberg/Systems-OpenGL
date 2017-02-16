#pragma once
#include "RenderableObject.h"

class Spline : public RenderableObject {
public:
	Spline(glm::vec3 p1, glm::vec3 p2, glm::vec4 color = { .5, .5, .5, 1 });
	~Spline();
	void renderFaces(GLenum usage) const;
	
	glm::vec3 vector() const;
	
	void movePoint(int idx, glm::vec3 displacement);
	void init();

private:
};