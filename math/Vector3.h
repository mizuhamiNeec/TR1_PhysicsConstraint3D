#pragma once
#include <cassert>

#include "Vec3.h"

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vec3 ConvertToVec3() const;

	Vector3 operator+(const Vec3& rhs) const;
	Vector3 operator-(const Vec3& rhs) const;
	Vector3 operator*(const Vec3& rhs) const;
	Vector3 operator/(const Vec3& rhs) const;

	/// <summary>
	/// 添字演算子
	/// </summary>
	float& operator[](const int index) {
		assert(index <= 2 && index >= 0 && "Index out of range");
		if (index == 0) {
			return x;
		}
		if (index == 1) {
			return y;
		}
		if (index == 2) {
			return z;
		}
		static float ret = 0.0f;
		return ret;
	}

	/// <summary>
	/// 添字演算子
	/// </summary>
	const float& operator[](const int index) const {
		assert(index <= 2 && index >= 0 && "Index out of range");
		if (index == 0) {
			return x;
		}
		if (index == 1) {
			return y;
		}
		if (index == 2) {
			return z;
		}
		static constexpr float ret = 0.0f;
		return ret;
	}
};

inline Vec3 Vector3::ConvertToVec3() const {
	Vec3 ret;
	for (int i = 0; i < 3; ++i) {
		ret[i] = (*this)[i];
	}
	return ret;
}

inline Vector3 Vector3::operator+(const Vec3& rhs) const {
	return {x + rhs.x, y + rhs.y, z + rhs.z};
}

inline Vector3 Vector3::operator-(const Vec3& rhs) const {
	return {x - rhs.x, y - rhs.y, z - rhs.z};
}

inline Vector3 Vector3::operator*(const Vec3& rhs) const {
	return {x * rhs.x, y * rhs.y, z * rhs.z};
}

inline Vector3 Vector3::operator/(const Vec3& rhs) const {
	return {x / rhs.x, y / rhs.y, z / rhs.z};
}