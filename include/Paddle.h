#pragma once

#include "RenderableObject.h"
#include "Scene.h"
#include "Block.h"

class Paddle : public RenderableObject{
public:
	Paddle(float x, float y, float width);
	Payload rayCollision(const glm::vec3& velocity, const glm::vec3& origin) const;
	virtual void translate(float x, float y, float z);

	Scene* scene;

private:
	bool moved_ = false;

};