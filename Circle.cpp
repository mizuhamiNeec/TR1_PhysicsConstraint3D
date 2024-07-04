#include "Circle.h"

#include "Circle.h"

#include "PrimitiveDrawer.h"

Circle::~Circle() {
}

bool Circle::IsCollide(const Circle& other) const {
	float distance = this->transform_.translation_.ConvertToVec3().Distance(other.transform_.translation_.ConvertToVec3());

	float radiusSum = this->circleRadius_ * this->transform_.scale_.x + other.circleRadius_ * other.transform_.scale_.x;
	return distance < radiusSum;
}

Circle::Circle(const std::string& name, const std::string& tag, const bool active, const float radius) : model_(nullptr) {
	transform_.Initialize();

	name_ = name;
	tag_ = tag;
	active_ = active;
	circleRadius_ = radius;
}

void Circle::Initialize(const std::string& name) {
	Object::Initialize(name);
	if (parent_) {
		maxDistanceToParent_ = transform_.translation_.ConvertToVec3().Distance(parent_->GetTransform().position);
	}
}

void Circle::ResolveCollision(Circle& other) {
	Vec3 normal = (other.transform_.translation_.ConvertToVec3() - this->transform_.translation_.ConvertToVec3()).Normalized();
	float relativeVelocity = (other.rb_.GetVelocity() - this->rb_.GetVelocity()).DotProduct(normal);

	// めり込み深度を計算
	float distance = (other.transform_.translation_.ConvertToVec3() - this->transform_.translation_.ConvertToVec3()).Length();
	float penetrationDepth = (this->circleRadius_ + other.circleRadius_) - distance;

	// 侵入している場合は位置を補正
	if (penetrationDepth > 0) {
		Vec3 correction = normal * (penetrationDepth / (this->rb_.GetMass() + other.rb_.GetMass()));
		for (int i = 0; i < 3; ++i) {
			this->transform_.translation_[i] -= correction[i] * (other.rb_.GetMass() / (this->rb_.GetMass() + other.rb_.GetMass()));
			other.transform_.translation_[i] += correction[i] * (this->rb_.GetMass() / (this->rb_.GetMass() + other.rb_.GetMass()));
		}
	}

	if (relativeVelocity > 0) {
		return;
	}

	float e = min(this->rb_.GetReboundCoefficient(), other.rb_.GetReboundCoefficient());
	float j = -(1 + e) * relativeVelocity;
	j /= 1 / this->rb_.GetMass() + 1 / other.rb_.GetMass();

	Vec3 impulse = normal * j;
	this->rb_.SetVelocity(this->rb_.GetVelocity() - impulse / this->rb_.GetMass());
	other.rb_.SetVelocity(other.rb_.GetVelocity() + impulse / other.rb_.GetMass());
}

void Circle::Update() {
	// スタティックだったら
	if (isStatic) {
		// 速度はゼロ
		rb_.SetVelocity(Vec3::zero);
	} else {
		// 剛体の更新
		rb_.Update(this, collisionShapes_);

		ApplyDistanceConstraint();

		// 子の当たり判定
		for (auto& child : children_) {
			if (auto circleChild = dynamic_cast<Circle*>(child.get())) {
				// 衝突していたら
				if (IsCollide(*circleChild)) {
					// 外に出す
					ResolveCollision(*circleChild);
				}

				// 他の子同士も判定を行う
				for (auto& otherChild : children_) {
					if (circleChild != otherChild.get()) {
						if (auto otherCircleChild = dynamic_cast<Circle*>(otherChild.get())) {
							if (circleChild->IsCollide(*otherCircleChild)) {
								circleChild->ResolveCollision(*otherCircleChild);
							}
						}
					}
				}
			}
		}
	}

	transform_.UpdateMatrix();

	// 子を更新
	for (auto& child : children_) {
		child->Update();
	}
}

void Circle::Draw(const ViewProjection& viewProjection) {

	model_->Draw(transform_, viewProjection);
	for (auto& child : children_) {
		child->Draw(viewProjection);
	}
}

void Circle::DebugDraw() const {
	if (bDrawDebug) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			transform_.translation_,
			transform_.translation_ + rb_.GetVelocity(),
			{1.0f,1.0f,0.0f,1.0f}
		);


		for (auto& child : children_) {
			//DrawLine(viewpos, camera.WorldToScreen(child->GetTransform().translation_), 0x00FF00FF);

			PrimitiveDrawer::GetInstance()->DrawLine3d(
				transform_.translation_,
				child->GetWorldTransform()->translation_,
				{1.0f,1.0f,1.0f,1.0f}
			);

			if (auto circle = dynamic_cast<Circle*>(child.get())) {
				{
					circle->DebugDraw();
				}
			}
		}
	}

}

void Circle::Details() {
	Object::Details();

	if (ImGui::CollapsingHeader("Circle", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat("Radius", &circleRadius_, 0.5f);
	}

	if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::Checkbox("IsStatic", &isStatic)) {
		}

		Vec3 tmpVel = rb_.GetVelocity();
		ImGui::DragFloat3("Velocity", &tmpVel.x, 1.0f);
		rb_.SetVelocity(tmpVel);

		ImGui::SameLine();
		if (ImGui::ArrowButton("##VelReset", ImGuiDir_Left)) {
			rb_.SetVelocity(Vec3::zero);
		}

		float tmpMass = rb_.GetMass();
		if (ImGui::DragFloat("Mass", &tmpMass, 0.01f)) {
			rb_.SetMass(tmpMass);
		}
	}

	if (ImGui::CollapsingHeader("Distance Constraint", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::DragFloat("MaxDistance", &maxDistanceToParent_, 0.1f)) {
		}

		if (ImGui::DragFloat("ReductionFactor", &reductionFactor, 0.001f)) {
		}
	}
}

Rigidbody Circle::GetRigidbody() const {
	return rb_;
}

bool Circle::GetStatic() const {
	return isStatic;
}

void Circle::ApplyDistanceConstraint() {
	// 親がある場合
	if (parent_) {
		// 子がCircleだった場合
		if (auto p = dynamic_cast<Circle*>(parent_.get())) {
			Vec3 direction;

			for (int i = 0; i < 3; ++i) {
				direction[i] = transform_.translation_[i] - parent_->GetTransform().position[i];
			}
			float currentDistance = direction.Length();

			// 最大距離より遠くにいたら
			if (currentDistance > maxDistanceToParent_) {
				Vec3 normal = direction.Normalized();
				Vec3 correction = normal * (currentDistance - maxDistanceToParent_);
				for (int i = 0; i < 3; ++i) {
					transform_.translation_[i] -= correction[i] * 0.5f;
				}

				// 範囲内に戻す
				if (!p->GetStatic()) {
					parent_->SetTransform(
						{parent_->GetTransform().position + correction * 0.5f},
						parent_->GetTransform().rotation,
						parent_->GetTransform().scale
					);
				}
			}

			if (currentDistance > 0.0f) {
				// Avoid division by zero
				Vec3 relativeVelocity = rb_.GetVelocity() - p->GetRigidbody().GetVelocity();
				Vec3 velocityAlongNormal = direction.Normalized() * relativeVelocity.DotProduct(
					direction.Normalized());
				rb_.SetVelocity(rb_.GetVelocity() - velocityAlongNormal * reductionFactor);
				p->GetRigidbody().SetVelocity(
					p->GetRigidbody().GetVelocity() + velocityAlongNormal * reductionFactor);
			}
		}
	}
}


void Circle::SetModel(Model* model) {
	model_ = model;
}
