#pragma once

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y);
	Vector2(const Vector2& v);

	Vector2 operator*(float n) const;
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;

	float m_x = 0.0f;
	float m_y = 0.0f;
};

