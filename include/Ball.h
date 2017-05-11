#pragma once

#include <glm/glm.hpp>

#include "Circle.h"
#include "Scene.h"


class Ball : public Circle {
public:
	Ball(float x, float y, float radius);

	void update(float dt, Scene* scene);
	void start(bool status);
	bool started() const { return started_; }
private:


	bool started_ = false;
	bool underwater_ = false;
};