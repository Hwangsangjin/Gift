#pragma once

class Vector3;

class Vector4
{
public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& v);
	Vector4(const Vector3& v);

	void Cross(Vector4& v1, Vector4& v2, Vector4 v3);

	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
	float m_w = 0.0f;
};

