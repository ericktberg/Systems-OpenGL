#include "Plane.h"

float lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}

/****************************************************************************************
* Constructors 
*****************************************************************************************/
Plane::Plane(GLenum mode, float width, float height, glm::vec4 color, int w_segments, int h_segments)
	: width_(width), height_(height), color_(color),
	w_segments_(w_segments + 1), h_segments_(h_segments + 1),
	RenderableObject(mode) 
{ 
	triangulate_ = true;

	/**********************************
	* Dimensions
	***********************************/
	float w_start = -width_ / 2;
	float h_start = -height_ / 2;
	float w_end = width_ / 2;
	float h_end = height_ / 2;

	/**********************************
	* Create even vertex grid
	***********************************/
	for (int w_idx = 0; w_idx < w_segments_; w_idx++) {
		for (int h_idx = 0; h_idx < h_segments_; h_idx++) {
			float w_step = (float)w_idx / (float)(w_segments_ - 1);
			float h_step = (float)h_idx / (float)(h_segments_ - 1);

			vertices_.push_back({ { lerp(w_start, w_end, w_step), 
									lerp(h_start, h_end, h_step), 
									0 }, color_, { 0, 0, 1 } });
		}
	}

	/**********************************
	* Find edges and faces
	***********************************/
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


Plane::Plane(GLenum mode, float width, float height,
			int w_segments, int h_segments) 
	: Plane(mode, width, height, {.5f, .5f, .5f, 1}, w_segments, h_segments) {}

Plane::Plane(GLenum mode, float width, float height, glm::vec4 color) : Plane(mode, width, height, color, 1, 1) {}
Plane::Plane(GLenum mode) : Plane(mode, 1, 1, 1, 1) {}

Plane::~Plane() {

}

/****************************************************************************************
* Geometry
*****************************************************************************************/
// TODO: revisit
std::vector<int> Plane::neighbors(int idx) const {
	std::vector<int> v;
	v.reserve(16);
	
	//	x	x	x	x	x
	//	x	ul	u	ur	x
	//	x	l	idx	r	x
	//	x	bl	b	br	x
	//	x	x	x	x	x
	
	bool u = idx % h_segments_ > 0;
	bool l = idx - h_segments_ >= 0;
	bool b = ((idx + 1) % h_segments_) != 0;
	bool r = idx + h_segments_ < h_segments_*w_segments_;

	if (u && l) {
		v.push_back(idx - h_segments_ - 1);
	}
	else { v.push_back(-1); }

	if (u) { // previous height
		v.push_back(idx - 1);
	}
	else { v.push_back(-1); }

	if (u && r) {
		v.push_back(idx + h_segments_ - 1);
	}
	else { v.push_back(-1); }

	if (r) { // next width
		v.push_back(idx + h_segments_);
	}
	else { v.push_back(-1); }

	if (b && r) { // previous width
		v.push_back(idx + h_segments_ + 1);
	}
	else { v.push_back(-1); }

	if (b) {
		v.push_back(idx + 1);
	}
	else {
		v.push_back(-1);
	}

	if (b && l) {
		v.push_back(idx - h_segments_ + 1);
	}
	else { v.push_back(-1); }

	if (l) { // previous width
		v.push_back(idx - h_segments_);
	}
	else { v.push_back(-1); }

	

	// bend springs
	u = idx % h_segments_ > 1;
	l = idx - h_segments_ * 2 >= 0;
	b = (idx + 2) % h_segments_ > 1;
	r = idx + h_segments_ * 2 < h_segments_*w_segments_;

	if (u && l) {
		v.push_back(idx - (h_segments_ - 1) * 2);
	}
	else { v.push_back(-1); }

	if (u) { // previous height
		v.push_back(idx - 2);
	}
	else { v.push_back(-1); }

	if (u && r) {
		v.push_back(idx + (h_segments_ - 1) * 2);
	}
	else { v.push_back(-1); }

	if (r) { // next width
		v.push_back(idx + h_segments_ * 2);
	}
	else { v.push_back(-1); }

	if (b && r) {
		v.push_back(idx + (h_segments_ + 1) * 2);
	}
	else { v.push_back(-1); }

	if (b) {
		v.push_back(idx + 2);
	}
	else {
		v.push_back(-1);
	}
	
	if (b && l) {
		v.push_back(idx - (h_segments_ + 1) * 2);
	}
	else { v.push_back(-1); }	

	if (l) { // previous width
		v.push_back(idx - h_segments_ * 2);
	}
	else { v.push_back(-1); }


	return v;
}