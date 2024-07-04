#pragma once
#include "CollisionShapes.h"
#include "Config.h"
#include "Object.h"
#include "Transform.h"
#include "Vec3.h"

class Rigidbody {
public:
	void AddForce(const Vec3 newForce) {
		force_ += newForce;
	}

	void Update(Object* object, CollisionShapes shapes) {
		shapes;

		AddForce({0.0f,-gravity,0.0f});

		Transform parentTransform = object->GetTransform();

		velocity_ += (force_ / mass_) * deltaTime;

		parentTransform.position += velocity_ * deltaTime;

		object->SetTransform(parentTransform.position, parentTransform.rotation, parentTransform.scale);
		force_ = Vec3::zero;
	}

	Vec3 GetVelocity() const {
		return velocity_;
	}

	void SetVelocity(const Vec3 newVel) {
		velocity_ = newVel;
	}

	float GetReboundCoefficient() const {
		return reboundCoefficient_;
	}

	float GetMass() const {
		return mass_;
	}

	void SetMass(float tmpMass) {
		mass_ = tmpMass;
	}

private:
	Vec3 velocity_; // 剛体の速度ベクトル
	float reboundCoefficient_ = 0.25f;
	float mass_ = 1.0f; // 剛体の重量

	Vec3 force_; // 剛体に与えるフォース
};
