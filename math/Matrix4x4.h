#pragma once
#include "Vector3.h"

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		Matrix4x4 s = MakeScaleMatrix(scale);
		Matrix4x4 rx = MakeRotateXMatrix(rotate.x);
		Matrix4x4 ry = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rz = MakeRotateZMatrix(rotate.z);
		Matrix4x4 t = MakeTranslateMatrix(translate);

		return s * rx * ry * rz * t;
	}

	// 5. 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m) {
		return {m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]};
	}

	// 6. 単位行列の作成
	static Matrix4x4 MakeIdentity4x4() { return {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }

	// 1. 平行移動行列
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate) { return {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translate.x, translate.y, translate.z, 1.0f}; }

	// 2. 拡大縮小行列
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale) { return {scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }

	// 3. 座標変換
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result; // w=1がデカルト座標系であるので(x,y,z,1)のベクトルとしてmatrixとの積をとる
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f); // ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
		result.x /= w;     // w=1がデカルト座標系であるので、w除算することで同時座標をデカルト座標に戻す
		result.y /= w;
		result.z /= w;
		return result;
	}

	// 1. X軸回転行列
	static Matrix4x4 MakeRotateXMatrix(const float radian) {
		Matrix4x4 result = MakeIdentity4x4();

		float cos = std::cos(radian);
		float sin = std::sin(radian);

		result.m[1][1] = cos;
		result.m[1][2] = sin;
		result.m[2][1] = -sin;
		result.m[2][2] = cos;

		return result;
	}

	// 2. Y軸回転行列
	static Matrix4x4 MakeRotateYMatrix(const float radian) {
		Matrix4x4 result = MakeIdentity4x4();

		float cos = std::cos(radian);
		float sin = std::sin(radian);

		result.m[0][0] = cos;
		result.m[0][2] = -sin;
		result.m[2][0] = sin;
		result.m[2][2] = cos;

		return result;
	}

	// 3. Z軸回転行列
	static Matrix4x4 MakeRotateZMatrix(const float radian) {
		Matrix4x4 result = MakeIdentity4x4();

		float cos = std::cos(radian);
		float sin = std::sin(radian);

		result.m[0][0] = cos;
		result.m[0][1] = sin;
		result.m[1][0] = -sin;
		result.m[1][1] = cos;

		return result;
	}

	static Matrix4x4 Inverse(const Matrix4x4& m) {
		const float a = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
			m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
			m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
			m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
			m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
			m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
			m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
			m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

		const Matrix4x4 result = {
			(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] -
			 m.m[1][1] * m.m[2][3] * m.m[3][2]) /
				a,
			(-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] +
			 m.m[0][1] * m.m[2][3] * m.m[3][2]) /
				a,
			(m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] -
			 m.m[0][1] * m.m[1][3] * m.m[3][2]) /
				a,
			(-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] +
			 m.m[0][1] * m.m[1][3] * m.m[2][2]) /
				a,

			(-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] +
			 m.m[1][0] * m.m[2][3] * m.m[3][2]) /
				a,
			(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] -
			 m.m[0][0] * m.m[2][3] * m.m[3][2]) /
				a,
			(-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] +
			 m.m[0][0] * m.m[1][3] * m.m[3][2]) /
				a,
			(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] -
			 m.m[0][0] * m.m[1][3] * m.m[2][2]) /
				a,

			(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] -
			 m.m[1][0] * m.m[2][3] * m.m[3][1]) /
				a,
			(-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] +
			 m.m[0][0] * m.m[2][3] * m.m[3][1]) /
				a,
			(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] -
			 m.m[0][0] * m.m[1][3] * m.m[3][1]) /
				a,
			(-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[0][1] * m.m[1][0] * m.m[2][3] +
			 m.m[0][0] * m.m[1][3] * m.m[2][1]) /
				a,

			(-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] +
			 m.m[1][0] * m.m[2][2] * m.m[3][1]) /
				a,
			(m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] -
			 m.m[0][0] * m.m[2][2] * m.m[3][1]) /
				a,
			(-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] +
			 m.m[0][0] * m.m[1][2] * m.m[3][1]) /
				a,
			(m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] -
			 m.m[0][0] * m.m[1][2] * m.m[2][1]) /
				a};

		return result;
	}

	static Matrix4x4 ViewportMat(const float left, const float top, const float width, const float height, const float minDepth, const float maxDepth) {
		Matrix4x4 result = MakeIdentity4x4();

		result.m[0][0] = width / 2.0f;
		result.m[1][1] = -height / 2.0f;
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + width / 2.0f;
		result.m[3][1] = top + height / 2.0f;
		result.m[3][2] = minDepth;

		return result;
	}

	Matrix4x4 operator+(const Matrix4x4& rhs) const {
		return {
			m[0][0] + rhs.m[0][0], m[0][1] + rhs.m[0][1], m[0][2] + rhs.m[0][2], m[0][3] + rhs.m[0][3], m[1][0] + rhs.m[1][0], m[1][1] + rhs.m[1][1], m[1][2] + rhs.m[1][2], m[1][3] + rhs.m[1][3],
			m[2][0] + rhs.m[2][0], m[2][1] + rhs.m[2][1], m[2][2] + rhs.m[2][2], m[2][3] + rhs.m[2][3], m[3][0] + rhs.m[3][0], m[3][1] + rhs.m[3][1], m[3][2] + rhs.m[3][2], m[3][3] + rhs.m[3][3],
		};
	}

	Matrix4x4 operator-(const Matrix4x4& rhs) const {
		return {
			m[0][0] - rhs.m[0][0], m[0][1] - rhs.m[0][1], m[0][2] - rhs.m[0][2], m[0][3] - rhs.m[0][3], m[1][0] - rhs.m[1][0], m[1][1] - rhs.m[1][1], m[1][2] - rhs.m[1][2], m[1][3] - rhs.m[1][3],
			m[2][0] - rhs.m[2][0], m[2][1] - rhs.m[2][1], m[2][2] - rhs.m[2][2], m[2][3] - rhs.m[2][3], m[3][0] - rhs.m[3][0], m[3][1] - rhs.m[3][1], m[3][2] - rhs.m[3][2], m[3][3] - rhs.m[3][3],
		};
	}

	Matrix4x4 operator*(const Matrix4x4& rhs) const {
		return {m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0],
				m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1],
				m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2],
				m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3],

				m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0],
				m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1],
				m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2],
				m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3],

				m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0],
				m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1],
				m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2],
				m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3],

				m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0],
				m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1],
				m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2],
				m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3]};
	}

	Matrix4x4& operator*=(const Matrix4x4& matrix4X4) {
		*this = *this * matrix4X4;
		return *this;
	}
};
