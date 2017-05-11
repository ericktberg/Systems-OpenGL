#pragma once
#include <glm/glm.hpp>
#include <iostream>
struct Payload {
	float t;
	int id;
	glm::vec3 normal;
	glm::vec3 velocity; 
	float cor;


	friend std::ostream& operator<<(std::ostream& stream, const Payload& payload) {
		stream << "id: " << payload.id << " t: " << payload.t << std::endl;
		return stream;
	}
};