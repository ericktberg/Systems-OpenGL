#pragma once

#include "RenderableObject.h"
#include "Plane.h"

#include <vector>

// simulation technique
#define ONE_DIMENSION 1
#define TWO_DIMENSION 2
// boundary conditions
#define REFLECT 3
#define PERIODIC 4
#define FREE 5


class ShallowWater {
public:
	ShallowWater(Plane* plane, int dimensions, int boundary_condition = PERIODIC);
	~ShallowWater();

	void update(float dt, const std::vector<RenderableObject*>& objects, int this_idx);
	void applyUpdate();
private:
	void update1D(float dt, const std::vector<RenderableObject*>& objects, int this_idx);
	void update2D(float dt, const std::vector<RenderableObject*>& objects, int this_idx);

	Plane* plane_;

	std::vector<float> height_,
					   momentum_;

	int dimension_, boundary_condition_;
};