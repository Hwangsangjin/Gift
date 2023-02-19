#pragma once

class Vector3;

class Vector4
{
public:
	Vector4() = default;
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& vector);
	Vector4(const Vector3& vector);

	void Cross(Vector4& v1, Vector4& v2, Vector4 v3);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

	void SetX(const float& x);
	void SetY(const float& y);
	void SetZ(const float& z);
	void SetW(const float& w);

private:
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
	float m_w = 0.0f;
};

