#include "Sphere.h"


Sphere::Sphere(float radius, glm::vec3 center, int subdiv) :
		radius_(radius), 
		center_(center),
		subdiv_(subdiv),
		RenderableObject()
{
}


Sphere::~Sphere()
{
}

void Sphere::init() {
	

	glm::vec4 white = { 1.f, 1.f, 1.f, 1 };
	glm::vec3 a = center_ + glm::vec3(radius_, 0.f, 0.f);
	glm::vec3 b = center_ + glm::vec3(-radius_, 0.f, 0.f);
	glm::vec3 c = center_ + glm::vec3(0.f, radius_, 0.f);
	glm::vec3 d = center_ + glm::vec3(0.f, -radius_, 0.f);
	glm::vec3 e = center_ + glm::vec3(0.f, 0.f, radius_);
	glm::vec3 f = center_ + glm::vec3(0.f, 0.f, -radius_);
	vertices_ = { { a, white }, { b, white }, { c, white },
	{ d, white }, { e, white }, { f, white } };

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


	int i, size;
	for (int s = 0; s < subdiv_; s++) {
		size = indices_.size();

		for (i = 0; i < size; i++) {
			subdivideTriangle(i);
		}
	}

	for (int idx = 0; idx < indices_.size(); idx++) {
		Face at = indices_.at(idx);
		edges_.push_back({ at.a, at.b });
		edges_.push_back({ at.a, at.c });
		edges_.push_back({ at.b, at.c });
	}
	
}

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

Vertex Sphere::midPoint(const int p1, const int p2) {
	Vertex v = (vertices_.at(p1) + vertices_.at(p2));
	glm::vec3 x = v.position - center_;
	return{ glm::normalize(x) * radius_ + center_, v.color };
}

bool Sphere::intersectsGround(const glm::vec3& groundPlane) {
	for (int i = 0; i < vertices_.size(); i++) {
		if (vertices_.at(i).position.z < groundPlane.z) {
			return true;
		}
	}
	return false;
}