#pragma once
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "Transform.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Camera;

class Object : public std::enable_shared_from_this<Object> {
public:
	virtual ~Object() = default;
	Transform GetTransform() {
		Transform ret;
		for (int i = 0; i < 3; ++i) {
			ret.position[i] = transform_.translation_[i];
			ret.rotation[i] = transform_.rotation_[i];
			ret.scale[i] = transform_.scale_[i];
		}

		return ret;
	}

	WorldTransform* GetWorldTransform() {
		return &transform_;
	}
	Object(std::string name = "Object", std::string tag = "", bool active = true);

	virtual void Initialize(const std::string& name);
	virtual void Update();
	virtual void Draw(const ViewProjection& viewProjection);
	virtual void Details();

	std::vector<std::shared_ptr<Object>> GetChildren();
	void SetTransform(Vec3 pos, Vec3 rotate, Vec3 scale);
	void SetTransform(WorldTransform& newTransform);
	std::string GetName();

	void SetParent(const std::shared_ptr<Object>& parentObject);

	void AddChild(const std::shared_ptr<Object>& child) {
		child->SetParent(shared_from_this());
		children_.push_back(child);
	}

	void RemoveChild(const std::shared_ptr<Object>& child) {
		std::erase(children_, child);
		child->SetParent(nullptr);
	}

protected:
	WorldTransform transform_;
	std::string name_;
	std::string tag_;
	bool active_;
	std::shared_ptr<Object> parent_;
	std::vector<std::shared_ptr<Object>> children_;

	bool maintainAspectRatio = false;
};
