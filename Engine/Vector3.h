#pragma once

class Vector3
{
public:
	Vector3() = default;
	Vector3(float x, float y, float z);
	Vector3(const Vector3& v);

	static Vector3 Lerp(const Vector3& start, const Vector3& end, float delta);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	Vector3 operator*(float n) const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3& operator+=(float n);
	Vector3& operator+=(const Vector3& v);

	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
};

