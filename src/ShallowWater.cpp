#include "ShallowWater.h"


inline float sqr(float x) {
	return x*x;
}

ShallowWater::
ShallowWater(Plane* plane, 
			 int dimensions, 
			 int boundary_condition
			 ) : plane_(plane), 
				dimension_(dimensions), 
				boundary_condition_(boundary_condition),
				height_(plane_->widthSegs(), .25),
				momentum_(plane_->widthSegs(), .25) {
	plane_->translate({ 0, 0, 1 });
	for (int j = 0; j < 3; j++) {
		height_.at(j) = (7.f - j * 2) / 10.f;
	}
}

ShallowWater::
~ShallowWater() {}

void ShallowWater::
update(float dt, const std::vector<RenderableObject*>& objects, int this_idx) {
	float timestep = .002; int range = ceil(dt / timestep);
	float dx = .1; // (plane_->width() / plane_->widthSegs());


	for (int step = 0; step < range; step++){
		if (dimension_ == ONE_DIMENSION) {
			update1D(timestep, objects, this_idx);
		}
		else if (dimension_ == TWO_DIMENSION) {
			update2D(timestep, objects, this_idx);
		}
	}
		applyUpdate();
}

void ShallowWater::
update1D(float dt, const std::vector<RenderableObject*>& objects, int this_idx) {
	std::vector<float> height_mid(plane_->widthSegs() - 1, 0);
	std::vector<float> momentum_mid(plane_->widthSegs() - 1, 0);
	float dx = .01; // (plane_->width() / plane_->widthSegs());

	float g = -.1;
	float damp = .01;

	for (int i = 0; i < plane_->widthSegs() - 1; i++) {

		// height midpoint
		height_mid.at(i) = (height_.at(i) + height_.at(i + 1)) * .5;
		// s
		height_mid.at(i) -= dt * .5 * (momentum_.at(i) + momentum_.at(i + 1)) / dx;

		// momentum midpoint
		momentum_mid.at(i) = (momentum_.at(i) + momentum_.at(i + 1)) * .5;
		momentum_mid.at(i) -= dt * .5 * (
			sqr(momentum_.at(i + 1))// / height_.at(i + 1)
			- sqr(momentum_.at(i))// / height_.at(i)
			+ .5 * g * sqr(height_.at(i + 1))
			- .5 * g * sqr(height_.at(i))
			) / dx;
	}

	for (int i = 0; i < plane_->widthSegs() - 2; i++) {
		height_.at(i + 1) -= dt * (momentum_mid.at(i + 1) - momentum_mid.at(i)) / dx;

		momentum_.at(i + 1) -= dt * (
			//damp * momentum_mid.at(i+1) +
			sqr(momentum_mid.at(i + 1))// / height_mid.at(i + 1)
			- sqr(momentum_mid.at(i))// / height_mid.at(i)
			+ .5 * g * sqr(height_mid.at(i + 1))
			- .5 * g * sqr(height_mid.at(i))
			) / dx;
	}
	// Maybe these need to be placed before the rest?
	if (boundary_condition_ == PERIODIC) {
		height_.at(0) = height_.at(height_.size() -2);
		height_.at(height_.size() - 1) = height_.at(height_.size() - 2);
		momentum_.at(0) = momentum_.at(momentum_.size() - 2);
		momentum_.at(momentum_.size() - 1) = momentum_.at(momentum_.size() - 2);
	}
	else if (boundary_condition_ == REFLECT) {
		height_.at(0) = height_.at(1);
		height_.at(height_.size() - 1) = height_.at(height_.size() - 2);
		momentum_.at(0) = -momentum_.at(1);
		momentum_.at(momentum_.size() - 1) = -momentum_.at(momentum_.size() - 2);
	}
}

void ShallowWater::
update2D(float dt, const std::vector<RenderableObject*>& objects, int this_idx) {

}

void ShallowWater::
applyUpdate() {
	for (int w = 0; w < plane_->widthSegs(); w++) {
		for (int h = 0; h < plane_->heightSegs(); h++) {
			int idx = h + w * plane_->heightSegs();
			glm::vec3 point = plane_->point(idx);
			plane_->editPoint(idx, glm::vec3( point.x, point.y, height_.at(w) ));
		}
	}

	plane_->calcNormals();
}
