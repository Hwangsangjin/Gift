#pragma once

class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y);
	Vector2(const Vector2& vector);

	float GetX() const;
	float GetY() const;

	Vector2 operator*(float scalar);
	Vector2 operator+(Vector2 vector);

private:
	float m_x = 0.0f;
	float m_y = 0.0f;
};

