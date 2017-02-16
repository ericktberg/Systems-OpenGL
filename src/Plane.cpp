#include "Plane.h"

Plane::Plane(float width, float height, glm::vec4 color, int w_segments, int h_segments) 
	: width_(width), height_(height), color_(color),
	w_segments_(w_segments), h_segments_(h_segments),
	RenderableObject() 
{ 
	triangulate_ = false;



	gravity_ = { 0, 0, -2 };
	velocity_ = { 0, 0, 0 };
}


Plane::Plane(float width, float height, 
			int w_segments, int h_segments) 
	: Plane(width, height, {.5f, .5f, .5f, 1}, w_segments, h_segments) {}

Plane::Plane(float width, float height, glm::vec4 color) : Plane(width, height, color, 1, 1) {}
Plane::Plane() : Plane(1, 1, 1, 1) {}

Plane::~Plane() {

}

//TODO make more readable.
void Plane::init() {
	float w_start = -width_ / 2;
	float h_start = -height_ / 2;
	float w_step = width_ / (w_segments_ - 1);
	float h_step = height_ / (h_segments_ - 1);

	for (int w_idx = 0; w_idx < w_segments_; w_idx++) {
		for (int h_idx = 0; h_idx < h_segments_; h_idx++) {
			vertices_.push_back({ { w_start + w_idx * w_step, h_start + h_idx * h_step, 0 }, color_ });
		}
	}
	int idx = 0;
	for (int i = 0; i < w_segments_; i++) {
		for (int j = 0; j < h_segments_; j++) {
			idx = j + i * h_segments_;
			if (idx + h_segments_ < w_segments_ * h_segments_) {
				edges_.push_back({ idx, idx + h_segments_ });
			}

			if (j < h_segments_ - 1) {
				edges_.push_back({ idx, idx + 1 });
			}

			if (idx + h_segments_ < w_segments_ * h_segments_ && j < h_segments_ - 1 && triangulate_) {
				edges_.push_back({ idx + 1, idx + h_segments_ });
			}

			if (idx + h_segments_ < w_segments_ * h_segments_ && j < h_segments_ - 1) {
				indices_.push_back({ idx, idx + h_segments_, idx + 1 });
				indices_.push_back({ idx + 1, idx + h_segments_, idx + h_segments_ + 1 });
			}
		}
	}
}

std::vector<int> Plane::neighbors(int idx) const {
	std::vector<int> v;
	v.reserve(4);
	if (((idx + 1) % h_segments_)) {
		v.push_back(idx + 1);
	}
	else {
		v.push_back(-1);
	}

	if ((idx % h_segments_)) {
		v.push_back(idx - 1);
	}
	else { v.push_back(-1); }

	if (idx + h_segments_ < h_segments_*w_segments_) {
		v.push_back(idx + h_segments_);
	}
	else { v.push_back(-1); }

	if (idx - h_segments_ >= 0) {
		v.push_back(idx - h_segments_);
	}
	else { v.push_back(-1); }

	return v;
}

void Plane::update(float dt, const Sphere& sphere) {
	velocity_ += gravity_ * dt;

	float t = sphere.collisionPoint(velocity_, position_);
	if ( t >= 0 && t < dt) {
		velocity_.z = 0;
		gravity_.z = 0;
	}
	translate(velocity_*dt);
}
