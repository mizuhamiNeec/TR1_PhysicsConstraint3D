#include "Mat4.h"

#include <cassert>
#include <cmath>
#include <format>

#include "Camera.h"

Mat4 Mat4::operator+(const Mat4& rhs) const {
	Mat4 result;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result.m[i][j] = m[i][j] + rhs.m[i][j];
	return result;
}

Mat4 Mat4::operator-(const Mat4& rhs) const {
	return {
		{
			{m[0][0] - rhs.m[0][0], m[0][1] - rhs.m[0][1], m[0][2] - rhs.m[0][2], m[0][3] - rhs.m[0][3]},
			{m[1][0] - rhs.m[1][0], m[1][1] - rhs.m[1][1], m[1][2] - rhs.m[1][2], m[1][3] - rhs.m[1][3]},
			{m[2][0] - rhs.m[2][0], m[2][1] - rhs.m[2][1], m[2][2] - rhs.m[2][2], m[2][3] - rhs.m[2][3]},
			{m[3][0] - rhs.m[3][0], m[3][1] - rhs.m[3][1], m[3][2] - rhs.m[3][2], m[3][3] - rhs.m[3][3]},
		},
	};
}

Mat4 Mat4::operator*(const Mat4& rhs) const {
	return {
		{
			{
				m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0],
				m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1],
				m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2],
				m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3]
			},
			{
				m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0],
				m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1],
				m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2],
				m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3]
			},
			{
				m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0],
				m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1],
				m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2],
				m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3]
			},
			{
				m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0],
				m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1],
				m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2],
				m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3]
			}
		},
	};
}

Mat4 Mat4::Inverse() const {
	const float a =
		m[0][0] * m[1][1] * m[2][2] * m[3][3] + m[0][0] * m[1][2] * m[2][3] * m[3][1] + m[0][0] * m[1][3] * m[2][1]
		* m[3][2] -
		m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3] - m[0][0] * m[1][1] * m[2][3]
		* m[3][2] -
		m[0][1] * m[1][0] * m[2][2] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1] - m[0][3] * m[1][0] * m[2][1]
		* m[3][2] +
		m[0][3] * m[1][0] * m[2][2] * m[3][1] + m[0][2] * m[1][0] * m[2][1] * m[3][3] + m[0][1] * m[1][0] * m[2][3]
		* m[3][2] +
		m[0][1] * m[1][2] * m[2][0] * m[3][3] + m[0][2] * m[1][3] * m[2][0] * m[3][1] + m[0][3] * m[1][1] * m[2][0]
		* m[3][2] -
		m[0][3] * m[1][2] * m[2][0] * m[3][1] - m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][1] * m[1][3] * m[2][0]
		* m[3][2] -
		m[0][1] * m[1][2] * m[2][3] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] - m[0][3] * m[1][1] * m[2][2]
		* m[3][0] +
		m[0][3] * m[1][2] * m[2][1] * m[3][0] + m[0][2] * m[1][1] * m[2][3] * m[3][0] + m[0][1] * m[1][3] * m[2][2]
		* m[3][0];

	const Mat4 result = {
		{
			{
				(m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][3] * m[
						2][2]
					* m
					[3][1] - m[1][2] * m[2][1] * m[3][3] - m[1][1] * m[2][3] * m[3][2]) / a,
				(-m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2] + m[0][3] * m[
						2][
						2] *
					m[3][1] + m[0][2] * m[2][1] * m[3][3] + m[0][1] * m[2][3] * m[3][2]) / a,
				(m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][3] * m[
						1][2]
					* m
					[3][1] - m[0][2] * m[1][1] * m[3][3] - m[0][1] * m[1][3] * m[3][2]) / a,
				(-m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2] + m[0][3] * m[
						1][
						2] *
					m[2][1] + m[0][2] * m[1][1] * m[2][3] + m[0][1] * m[1][3] * m[2][2]) / a
			},
			{
				(-m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2] + m[1][3] * m[
						2][
						2] *
					m[3][0] + m[1][2] * m[2][0] * m[3][3] + m[1][0] * m[2][3] * m[3][2]) / a,
				(m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][3] * m[
						2][2]
					* m
					[3][0] - m[0][2] * m[2][0] * m[3][3] - m[0][0] * m[2][3] * m[3][2]) / a,
				(-m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2] + m[0][3] * m[
						1][
						2] *
					m[3][0] + m[0][2] * m[1][0] * m[3][3] + m[0][0] * m[1][3] * m[3][2]) / a,
				(m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][3] * m[
						1][2]
					* m
					[2][0] - m[0][2] * m[1][0] * m[2][3] - m[0][0] * m[1][3] * m[2][2]) / a
			},

			{
				(m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][3] * m[
						2][1]
					* m
					[3][0] - m[1][1] * m[2][0] * m[3][3] - m[1][0] * m[2][3] * m[3][1]) / a,
				(-m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1] + m[0][3] * m[
						2][
						1] *
					m[3][0] + m[0][1] * m[2][0] * m[3][3] + m[0][0] * m[2][3] * m[3][1]) / a,
				(m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][3] * m[
						1][1]
					* m
					[3][0] - m[0][1] * m[1][0] * m[3][3] - m[0][0] * m[1][3] * m[3][1]) / a,
				(-m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1] + m[0][3] * m[
						1][
						1] *
					m[2][0] * m[0][1] * m[1][0] * m[2][3] + m[0][0] * m[1][3] * m[2][1]) / a
			},

			{
				(-m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1] + m[1][2] * m[
						2][
						1] *
					m[3][0] + m[1][1] * m[2][0] * m[3][2] + m[1][0] * m[2][2] * m[3][1]) / a,
				(m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][2] * m[
						2][1]
					* m
					[3][0] - m[0][1] * m[2][0] * m[3][2] - m[0][0] * m[2][2] * m[3][1]) / a,
				(-m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1] + m[0][2] * m[
						1][
						1] *
					m[3][0] + m[0][1] * m[1][0] * m[3][2] + m[0][0] * m[1][2] * m[3][1]) / a,
				(m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[
						1][1]
					* m
					[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1]) / a
			}
		},
	};

	return result;
}

Mat4 Mat4::Transpose() const {
	return {
		{
			{m[0][0], m[1][0], m[2][0], m[3][0]},
			{m[0][1], m[1][1], m[2][1], m[3][1]},
			{m[0][2], m[1][2], m[2][2], m[3][2]},
			{m[0][3], m[1][3], m[2][3], m[3][3]}
		}
	};
}

Mat4 Mat4::Identity() {
	return {
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
}

Mat4 Mat4::Translate(const Vec3& translate) {
	return {
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{translate.x, translate.y, translate.z, 1.0f}
		},
	};
}

Mat4 Mat4::Scale(const Vec3& scale) {
	return {
		{
			{scale.x, 0.0f, 0.0f, 0.0f},
			{0.0f, scale.y, 0.0f, 0.0f},
			{0.0f, 0.0f, scale.z, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
	};
}

Vec3 Mat4::Transform(const Vec3& vector, const Mat4& matrix) {
	Vec3 result; // w=1がデカルト座標系であるので(x,y,z,1)のベクトルとしてmatrixとの積をとる
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][
		0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][
		1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][
		2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f); // ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
	result.x /= w; // w=1がデカルト座標系であるので、w除算することで同時座標をデカルト座標に戻す
	result.y /= w;
	result.z /= w;
	return result;
}

Mat4 Mat4::RotateX(const float radian) {
	Mat4 result = Identity();

	const float cos = std::cos(radian);
	const float sin = std::sin(radian);

	result.m[1][1] = cos;
	result.m[1][2] = sin;
	result.m[2][1] = -sin;
	result.m[2][2] = cos;

	return result;
}

Mat4 Mat4::RotateY(const float radian) {
	Mat4 result = Identity();

	const float cos = std::cos(radian);
	const float sin = std::sin(radian);

	result.m[0][0] = cos;
	result.m[0][2] = -sin;
	result.m[2][0] = sin;
	result.m[2][2] = cos;

	return result;
}

Mat4 Mat4::RotateZ(const float radian) {
	Mat4 result = Identity();

	const float cos = std::cos(radian);
	const float sin = std::sin(radian);

	result.m[0][0] = cos;
	result.m[0][1] = sin;
	result.m[1][0] = -sin;
	result.m[1][1] = cos;

	return result;
}

Mat4 Mat4::Affine(const Vec3& scale, const Vec3& rotate, const Vec3& translate) {
	const Mat4 s = Scale(scale);
	const Mat4 rx = RotateX(rotate.x);
	const Mat4 ry = RotateY(rotate.y);
	const Mat4 rz = RotateZ(rotate.z);
	const Mat4 t = Translate(translate);

	return s * rx * ry * rz * t;
}

Mat4 Mat4::PerspectiveFovMat(const float fov, const float aspectRatio, const float nearClip, const float farClip,
	FovAxis fovAxis) {
	Mat4 result = Identity();

	float cot;

	switch (fovAxis) {
	case FovAxis::Horizontal:
		cot = 1.0f / std::tan(fov * 0.5f);
		result.m[0][0] = cot / aspectRatio;
		result.m[1][1] = cot;
		break;
	case FovAxis::Vertical:
		cot = 1.0f / std::tan(fov * 0.5f);
		result.m[0][0] = cot;
		result.m[1][1] = cot * aspectRatio;
		break;
	}

	const float dist = farClip - nearClip;

	result.m[2][2] = (farClip + nearClip) / dist;
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / dist;

	return result;
}

Mat4 Mat4::OrthographicMat(const float left, const float top, const float right, const float bottom,
	const float nearClip, const float farClip) {
	Mat4 result = Identity();

	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);

	return result;
}

Mat4 Mat4::ViewportMat(const float left, const float top, const float width, const float height, const float minDepth,
	const float maxDepth) {
	Mat4 result = Identity();

	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;

	return result;
}
