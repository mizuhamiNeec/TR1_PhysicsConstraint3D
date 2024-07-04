#include "Camera.h"

#include <imgui.h>
#include <string>
#include <utility>

#include "Config.h"
#include "Mat4.h"

Camera::Camera(std::string name, std::string tag, bool active) {
	name_ = std::move(name);
	tag_ = std::move(tag);
	active_ = active;
}

void Camera::Initialize(const std::string& name) {
	Object::Initialize(name);
}

void Camera::SetViewProjection(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void Camera::Update() {
	Object::Update();
	viewProjection_->translation_ = transform_.translation_;
	viewProjection_->rotation_ = transform_.rotation_;
	viewProjection_->UpdateMatrix(); // ビュー行列の更新
}

ViewProjection* Camera::GetViewProjection() const {
	return viewProjection_;
}

Vec3 Camera::TransformNormal(const Vec3 v, const Mat4& m) {
	return {
		   v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		   v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
}

void Camera::Details() {
	Object::Details();

	ImGui::Begin("Details");

	if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		float newFov = viewProjection_->fovAngleY * rad2Deg;
		if (ImGui::DragFloat("Fov", &newFov, 1.0f, 1.0f, 180.0f)) {
			viewProjection_->fovAngleY = newFov * deg2Rad;
		}
	}

	ImGui::End();
}

float Camera::GetZoom() const {
	return zoom_;
}
