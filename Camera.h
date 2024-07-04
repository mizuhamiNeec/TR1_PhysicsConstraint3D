#pragma once
#include "Mat4.h"
#include "Object.h"
#include "Vec3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

enum class ProjectionMode {
	Perspective,
	Orthographic
};

class Camera : public Object {
public:
	Camera(std::string name = "Camera", std::string tag = "", bool active = true);

	void Initialize(const std::string& name) override;
	void SetViewProjection(ViewProjection* viewProjection);
	void Update() override;

	ViewProjection* GetViewProjection() const;

	static Vec3 TransformNormal(const Vec3 v, const Mat4& m);

	void Details() override;

	float GetZoom() const;

private:
	float zoom_ = 0.25f;

	const WorldTransform* target_ = nullptr;

	ViewProjection* viewProjection_;
};
