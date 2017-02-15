#include "Projectile.h"
#include "Sphere.h"
Projectile::Projectile(glm::vec3 init_velocity, glm::vec3 gravity) : velocity_(init_velocity), DynamicObject(gravity) {
}

Projectile::Projectile(glm::vec3 init_velocity) : Projectile(init_velocity, { 0, 0, -9.8 }) {
}

Projectile::~Projectile() {

}

void Projectile::init(RenderableObject* object) {
	object_ = object;
}

void Projectile::update(float dt) {
	velocity_ += gravity_ * dt;
	if (object_->intersectsGround({ 0, 0, 0 })) {
		velocity_.z = .8 * abs(velocity_.z);
	}
	object_->translate(velocity_*dt);

}
