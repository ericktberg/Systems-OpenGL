#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
};

inline Vertex operator+(Vertex p1, Vertex p2) {
	return { (p1.position + p2.position) / 2.f, (p1.color + p2.color) / 2.f };
}

inline Vertex operator/(Vertex p, float div) {
	return { p.position / div, p.color };
}