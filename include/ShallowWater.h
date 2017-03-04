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
	ShallowWater(Plane* plane, int dimensions, int boundary_condition = REFLECT);
	~ShallowWater();

	void update(float dt, const std::vector<RenderableObject*>& objects, int this_idx);
	void applyUpdate();
private:
	void update1D(float dt, const std::vector<RenderableObject*>& objects, int this_idx);
	void update2D(float dt, const std::vector<RenderableObject*>& objects, int this_idx);

	//----------------------------------------------------------------------------
	// index into 1d array using 2d coordinates
	int idx(int i, int j) const;

	int x_segs_, y_segs_;

	Plane* plane_;

	std::vector<float> h_,
					   uh_, vh_;

	int dimension_, boundary_condition_;
};