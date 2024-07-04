#include "Object.h"

#include <imgui.h>

#include "Camera.h"

std::vector<std::shared_ptr<Object>> Object::GetChildren() {
	return children_;
}

void Object::SetParent(const std::shared_ptr<Object>& parentObject) {
	parent_ = parentObject;
}

void Object::SetTransform(Vec3 pos, Vec3 rotate, Vec3 scale) {
	for (int i = 0; i < 3; ++i) {
		transform_.translation_[i] = pos[i];
		transform_.rotation_[i] = rotate[i];
		transform_.scale_[i] = scale[i];
	}
}

void Object::SetTransform(WorldTransform& newTransform) {
	for (int i = 0; i < 3; ++i) {
		transform_.translation_[i] = newTransform.translation_[i];
		transform_.rotation_[i] = newTransform.rotation_[i];
		transform_.scale_[i] = newTransform.scale_[i];
	}
}

Object::Object(std::string name, std::string tag, const bool active) : name_(std::move(name)), tag_(std::move(tag)),
active_(active), parent_(nullptr) {
}

void Object::Initialize(const std::string& name) {
	name_ = name;
	transform_.Initialize();
}

void Object::Update() {
	for (auto& child : children_) {
		child->Update();
	}
}

void Object::Draw(const ViewProjection& viewProjection) {
	viewProjection;
}

void Object::Details() {
	ImGui::Begin("Details");
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Location ", &transform_.translation_.x, 0.01f);
		ImGui::SameLine();
		if (ImGui::ArrowButton("LocReset", ImGuiDir_Left)) {
			transform_.translation_ = {0.0f,0.0f,0.0f};
		}

		ImGui::DragFloat3("Rotation ", &transform_.rotation_.x, 0.01f);
		ImGui::SameLine();
		if (ImGui::ArrowButton("RotReset", ImGuiDir_Left)) {
			transform_.rotation_ = {0.0f,0.0f,0.0f};
		}

		Vector3 initScale = transform_.scale_;

		// DragFloat3を使用してscaleを編集
		if (ImGui::DragFloat3("Scale", &transform_.scale_.x, 0.01f)) {
			if (maintainAspectRatio && initScale.x != 0.0f && initScale.y != 0.0f && initScale.z != 0.0f) {
				// 比率を維持するための処理
				float ratioX = transform_.scale_.x / initScale.x;
				float ratioY = transform_.scale_.y / initScale.y;
				float ratioZ = transform_.scale_.z / initScale.z;

				// 変化した方向を検出
				Vector3 dir;
				for (int i = 0; i < 3; ++i) {
					dir[i] = transform_.scale_[i] - initScale[i];
				}

				// 変化した比率を決定
				float ratio = 1.0f;
				if (dir.x != 0.0f) ratio = ratioX;
				else if (dir.y != 0.0f) ratio = ratioY;
				else if (dir.z != 0.0f) ratio = ratioZ;

				// スケールを更新
				transform_.scale_.x = initScale.x * ratio;
				transform_.scale_.y = initScale.y * ratio;
				transform_.scale_.z = initScale.z * ratio;
			}
		}

		ImGui::SameLine(0, 9);

		if (ImGui::Checkbox("##AspectRatio", &maintainAspectRatio)) {
		}

		ImGui::SameLine();
		if (ImGui::ArrowButton("ScaleReset", ImGuiDir_Left)) {
			transform_.scale_ = {1.0f,1.0f,1.0f};
		}
	}

	ImGui::End();
}

std::string Object::GetName() {
	return name_;
}
