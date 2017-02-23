#include "Sphere.h"
#include <glm/gtx/norm.hpp>
#include <algorithm>
Sphere::Sphere(GLenum mode, float radius, glm::vec3 center, int subdiv) :
		radius_(radius), 
		center_(center),
		subdiv_(subdiv),
		RenderableObject(mode)
{
	/**********************************
	* Create Octohedron
	***********************************/
	glm::vec4 white = { .8f, .8f, .8f, 1 };
	glm::vec3 a = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 b = glm::vec3(-1.f, 0.f, 0.f);
	glm::vec3 c = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 d = glm::vec3(0.f, -1.f, 0.f);
	glm::vec3 e = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 f = glm::vec3(0.f, 0.f, -1.f);
	vertices_ = { { radius_*a, white, a }, { radius_*b, white, b }, { radius_*c, white, c },
	{ radius_*d, white, d }, { radius_*e, white, e }, { radius_*f, white, f } };

	indices_ = {
		{ 0, 4, 2 },
		{ 2, 4, 1 },
		{ 1, 4, 3 },
		{ 3, 4, 0 },
		{ 5, 0, 2 },
		{ 5, 2, 1 },
		{ 5, 1, 3 },
		{ 5, 3, 0 }
	};

	/**********************************
	* Subdivide triangles (create indices in process)
	***********************************/
	int i, size;
	for (int s = 0; s < subdiv_; s++) {
		size = indices_.size();

		for (i = 0; i < size; i++) {
			subdivideTriangle(i);
		}
	}

	/**********************************
	* Find edges
	***********************************/
	for (int idx = 0; idx < indices_.size(); idx++) {
		Face at = indices_.at(idx);
		edges_.push_back({ at.a, at.b });
		edges_.push_back({ at.a, at.c });
		edges_.push_back({ at.b, at.c });
	}

	/**********************************
	* Move translation coordinates
	***********************************/
	translate(center_);

	// HACK for collisions
	radius_ = radius_ + .1;
}

Sphere::Sphere(GLenum mode, float radius, glm::vec3 center) : Sphere(mode, radius, center, 2) {}
Sphere::Sphere(GLenum mode, float radius) : Sphere(mode, radius, { 0, 0, 0 }) {}
Sphere::Sphere(GLenum mode) : Sphere(mode, .25) {}


Sphere::~Sphere()
{
}

//----------------------------------------------------------------------------
// Turn one triangle into 4
void Sphere::subdivideTriangle(int face_idx) {
	Face face = indices_.at(face_idx);
	int p1 = face.a;
	int p2 = face.b;
	int p3 = face.c;

	Vertex v4 = midPoint(p1, p2);
	Vertex v5 = midPoint(p1, p3);
	Vertex v6 = midPoint(p2, p3);

	int p4 = vertices_.size();
	int p5 = p4 + 1;
	int p6 = p5 + 1;

	vertices_.push_back(v4);
	vertices_.push_back(v5);
	vertices_.push_back(v6);

	indices_.at(face_idx) = { p1, p4, p5 };
	indices_.push_back({ p4, p6, p5 });
	indices_.push_back({ p4, p2, p6 });
	indices_.push_back({ p6, p3, p5 });	
}

//----------------------------------------------------------------------------
// Calculate the midpoint of a line
Vertex Sphere::midPoint(const int p1, const int p2) {
	Vertex v = (vertices_.at(p1) + vertices_.at(p2));
	glm::vec3 x = v.position;
	return{ glm::normalize(x) * radius_, v.color, glm::normalize(x) };
}

//----------------------------------------------------------------------------
// Will it intersect the ground?
bool Sphere::intersectsGround(const glm::vec3& groundPlane) {
	glm::mat4 transp = translation_ * rotation_ * scale_;
	for (int i = 0; i < vertices_.size(); i++) {
		if ((transp * glm::vec4(vertices_.at(i).position, 1)).z < groundPlane.z) {
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------
// Return the time parameter "t" of a ray-sphere intersection
//		Returns -1 on non-intersection
float Sphere::collisionPoint(const glm::vec3& vector, const glm::vec3& position) const {
	float A = glm::dot(vector, vector);
	float B = 2 * glm::dot(vector, (position - center_));
	float C = glm::dot(position - center_, position - center_) - radius_ * radius_;

	float discriminant = B * B - 4 * A * C;
	float t;
	if (discriminant >= 0) {
		t = std::min((-B + sqrt(discriminant)) / (2 * A), (-B - sqrt(discriminant)) / ( 2 * A));
	}
	else {
		t = -1;
	}

	return t;
}

