#pragma once

#include "RenderableObject.h"
#include "Sphere.h"

class Plane : public RenderableObject {
public:
	Plane(float width, float height, glm::vec4 color, int w_segments, int h_segments);
	Plane(float width, float height, int w_segments, int h_segments);
	Plane(float width, float height, glm::vec4 color);
	Plane();

	~Plane();
	void init();
	std::vector<int> neighbors(int idx) const;
	void update(float dt, const Sphere& sphere);

private:
	float width_, height_;
	int w_segments_, h_segments_;
	bool triangulate_; //for displaying triangles in edge view.
	glm::vec4 color_;


	glm::vec3 velocity_;
	glm::vec3 gravity_;
};