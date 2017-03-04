#include "ShallowWater.h"


inline float sqr(double x) {
	return x*x;
}

ShallowWater::
ShallowWater(Plane* plane, 
			 int dimensions, 
			 int boundary_condition
			 ) : plane_(plane), 
				dimension_(dimensions), 
				boundary_condition_(boundary_condition),
				x_segs_(plane->widthSegs()), y_segs_(plane->heightSegs()),
				h_(x_segs_ 
					* (dimensions == ONE_DIMENSION ? 1 : y_segs_)
					, 7),
				uh_(x_segs_
					* (dimensions == ONE_DIMENSION ? 1 : y_segs_)
					, 0),
				vh_(x_segs_
					* (dimensions == ONE_DIMENSION ? 1 : y_segs_)
					, 0)
{
	plane_->translate({ 0, 0, 1 });
	h_.at(10) += 10;
	h_.at(11) += 6;
	h_.at(9) += 6;
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
	std::vector<float> hm(x_segs_ - 1, 0);
	std::vector<float> uhm(x_segs_ - 1, 0);
	float dx = .1; // (plane_->width() / plane_->widthSegs());

	float g = 1;
	float damp = .1;

	if (boundary_condition_ == REFLECT) {
		h_[0] = h_[1];
		h_[x_segs_ - 1] = h_[x_segs_ - 2];
		uh_[0] = -uh_[1];
		uh_[x_segs_ - 1] = -uh_[x_segs_ - 2];
	}

	/**********************************
	* Calculate midpoints
	***********************************/
	for (int i = 0; i < x_segs_ - 1; i++) {
		hm[i] = (h_[i] + h_[i + 1]) / 2.0 - (dt / 2.0)*(uh_[i] + uh_[i + 1]) / dx;
		uhm[i] = (uh_[i] + uh_[i + 1]) / 2.0 - (dt / 2.0)*(
			sqr(uh_[i + 1]) / h_[i + 1] + .5*g*sqr(h_[i + 1]) -
			sqr(uh_[i]) / h_[i] - .5*g*sqr(h_[i])
			) / dx;
	}

	/**********************************
	* Calculate Full step
	***********************************/
	for (int i = 0; i < x_segs_ - 2; i++) {
		h_[i + 1] -= dt * (uhm[i + 1] - uhm[i]) / dx;
		uh_[i + 1] -= dt*(damp*uh_[i+1] +
			sqr(uhm[i + 1]) / hm[1 + 1] + .5*g*sqr(hm[i + 1]) -
			sqr(uhm[i]) / hm[i] - .5*g*sqr(hm[i])) / dx;
	}

}

void ShallowWater::
update2D(float dt, const std::vector<RenderableObject*>& objects, int this_idx) {
	std::vector<float> x_hm(x_segs_*(y_segs_ - 1), 0);
	std::vector<float> x_uhm(x_segs_*(y_segs_ - 1), 0);
	std::vector<float> x_vhm(x_segs_*(y_segs_ - 1), 0);
	std::vector<float> y_hm(x_segs_*(y_segs_ - 1), 0);
	std::vector<float> y_uhm(x_segs_*(y_segs_ - 1), 0);
	std::vector<float> y_vhm(x_segs_*(y_segs_ - 1), 0);

	float dx = .1; // (plane_->width() / plane_->widthSegs());
	float dy = .1;

	float g = .5;
	float damp = .1;
	float d2 = dt * .5;

	//if (boundary_condition_ == REFLECT) {
	//	h_[0] = h_[1];
	//	h_[x_segs_ - 1] = h_[x_segs_ - 2];
	//	uh_[0] = -uh_[1];
	//	uh_[x_segs_ - 1] = -uh_[x_segs_ - 2];
	//}

	/**********************************
	* Calculate midpoints
	***********************************/
	/*  % x direction
       i = 1:n+1;
       j = 1:n;
   
       % height
       Hx(i,j) = (H(i+1,j+1)+H(i,j+1))/2 - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
   
       % x momentum
       Ux(i,j) = (U(i+1,j+1)+U(i,j+1))/2 -  ...
                 dt/(2*dx)*((U(i+1,j+1).^2./H(i+1,j+1) + g/2*H(i+1,j+1).^2) - ...
                            (U(i,j+1).^2./H(i,j+1) + g/2*H(i,j+1).^2));
   
       % y momentum
       Vx(i,j) = (V(i+1,j+1)+V(i,j+1))/2 - ...
                 dt/(2*dx)*((U(i+1,j+1).*V(i+1,j+1)./H(i+1,j+1)) - ...
                            (U(i,j+1).*V(i,j+1)./H(i,j+1)));
       */

	for (int m = 0; m < x_segs_ - 1; m++) {
		for (int n = 0; n < y_segs_ - 2; n++) {
			int i = idx(m, n);
			int q = idx(m + 1, n+1);
			int p = idx(m, n + 1);

			x_hm[i] = (h_[q] + h_[p]) * .5 - (d2)*(uh_[q] + uh_[p]) / dx;

			x_uhm[i] = (uh_[q] + uh_[p]) * .5 -
				(d2)*(
				sqr(uh_[q]) / h_[q] + g*sqr(h_[q]) -
				sqr(uh_[p]) / h_[p] - g*sqr(h_[p])
				) / dx;
			x_vhm[i] = (vh_[q] + vh_[p]) * .5 -
				(d2)*(
				(vh_[q] * uh_[q]) / h_[q] -
				(vh_[p] * uh_[p]) / h_[p]
				) / dy;;
		}
	}

	for (int m = 0; m < x_segs_ - 2; m++) {
		for (int n = 0; n < y_segs_ - 1; n++) {
			int i = idx(m, n);
			int q = idx(m + 1, n + 1);
			int p = idx(m + 1, n);

			y_hm[i] = (h_[q] + h_[p]) * .5 - (d2)*(vh_[q] + vh_[p]) / dy;

			y_uhm[i] = (vh_[q] + vh_[p]) * .5 - 
				(d2)*(
				(vh_[q] * uh_[q] / h_[q]) -
				(vh_[p] * uh_[p] / h_[p])
				) / dy;
			y_vhm[i] = (vh_[q] + vh_[p]) * .5 -
				(d2)*(
				(sqr(vh_[q]) / h_[q]) + g*sqr(h_[q])) -
				(sqr(vh_[p]) / h_[p] - g*sqr(h_[p])
				) / dy;;
		}
	}

	/**********************************
	* Calculate Full step
	***********************************/
	for (int m = 0; m < x_segs_ - 1; m++) {
		for (int n = 0; n < y_segs_ - 1; n++) {
			int i = idx(m, n);
			int q = idx(m + 1, n + 1);
			int p = idx(m + 1, n);
			int r = idx(m, n + 1);

			h_[q] -= dt * (x_uhm[p] - x_uhm[i]) / dx -
					 dt * (y_vhm[r] - y_vhm[i]) / dy;

			uh_[q] -= dt*(
				sqr(x_uhm[p]) / x_hm[p] + g*sqr(x_hm[p]) -
				sqr(x_uhm[i]) / x_hm[i] - g*sqr(x_hm[i])) / dx
				- dt * (
				(y_uhm[r] * y_vhm[r]) / y_hm[r] -
				(y_uhm[i] * y_vhm[i]) / y_hm[i]) / dy;

			vh_[q] -= dt*(
				(y_uhm[p] * x_uhm[p]) / x_hm[p] -
				(y_uhm[i] * x_uhm[i]) / x_hm[i]) / dx
				- dt * (
				sqr(y_uhm[r]) / y_hm[r] + g*sqr(y_hm[r]) -
				sqr(y_uhm[i]) / y_hm[i] - g*sqr(y_hm[i])) / dy;

		}
	}


}

int ShallowWater::
idx(int i, int j) const {
	return i * (y_segs_ - 1) + j;
}

void ShallowWater::
applyUpdate() {
	for (int w = 0; w < x_segs_; w++) {
		if (dimension_ == ONE_DIMENSION) {
			float height = h_.at(w);

			if (height != height) { // is NaN
				return;
			}
			for (int h = 0; h < y_segs_; h++) {
				int i = w * y_segs_ + h;
				glm::vec3 point = plane_->point(i);


 				plane_->editPoint(i, glm::vec3(point.x, point.y, height));
			}
		}
	}

	plane_->calcNormals();
}
