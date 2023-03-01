#include "pch.h"
#include "Vector2.h"

Vector2::Vector2(float x, float y)
	: m_x(x)
	, m_y(y)
{
}

Vector2::Vector2(const Vector2& v)
	: m_x(v.m_x)
	, m_y(v.m_y)
{
}

Vector2 Vector2::operator*(float n) const
{
	return Vector2(m_x * n, m_y * n);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(m_x + v.m_x, m_y + v.m_y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(m_x - v.m_x, m_y - v.m_y);
}