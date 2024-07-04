#pragma once
#include <imgui.h>

#include "Camera.h"
#include "CollisionShapes.h"
#include "Model.h"
#include "Object.h"
#include "Rigidbody.h"

class Circle final : public Object {
public:
	~Circle() override;

	float GetRadius() const {
		return circleRadius_;
	}

	bool IsCollide(const Circle& other) const;

	Circle(const std::string& name = "Circle", const std::string& tag = "", bool active = true, float radius = 1.0f);
	void Initialize(const std::string& name) override;

	void SetModel(Model* model);

	void ResolveCollision(Circle& other);

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;
	void DebugDraw() const;

	void Details() override;

	Rigidbody GetRigidbody() const;

	bool GetStatic() const;

	void ApplyDistanceConstraint();

private:
	float circleRadius_ = 1.0f;

	Rigidbody rb_;
	CollisionShapes collisionShapes_;

	float maxDistanceToParent_ = 0.0f;

	bool isStatic = false;

	Model* model_;
};
