#pragma once

#include "RenderableObject.h"
#include "Sphere.h"

class Plane : public RenderableObject {
public:
	//----------------------------------------------------------------------------
	// Constructors
	Plane(GLenum mode, float width, float height, glm::vec4 color, int w_segments, int h_segments);
	Plane(GLenum mode, float width, float height, int w_segments, int h_segments);
	Plane(GLenum mode, float width, float height, glm::vec4 color);
	Plane(GLenum mode);
	~Plane();

	//----------------------------------------------------------------------------
	// Information
	std::vector<int> neighbors(int idx) const;

	//----------------------------------------------------------------------------
	// Geometry
	void updateNormal(int idx, glm::vec3 normal) { vertices_.at(idx).normal = normal; }

private:
	//----------------------------------------------------------------------------
	// Shape parameters
	float width_, height_;
	int w_segments_, h_segments_;
	glm::vec4 color_;

	//----------------------------------------------------------------------------
	// Render
	bool triangulate_; //for displaying triangles in edge view.
};