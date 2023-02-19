#include "pch.h"
#include "Vector2.h"

Vector2::Vector2(float x, float y)
	: m_x(x)
	, m_y(y)
{
}

Vector2::Vector2(const Vector2& vector)
	: m_x(vector.m_x)
	, m_y(vector.m_y)
{
}

float Vector2::GetX() const
{
	return m_x;
}

float Vector2::GetY() const
{
	return m_y;
}

Vector2 Vector2::operator*(float scalar)
{
	return Vector2(m_x * scalar, m_y * scalar);
}

Vector2 Vector2::operator+(const Vector2& vector)
{
	return Vector2(m_x + vector.m_x, m_y + vector.m_y);
}
