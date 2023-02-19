#include "pch.h"
#include "Vector3.h"

Vector3::Vector3(float x, float y, float z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

Vector3::Vector3(const Vector3& vector)
	: m_x(vector.m_x)
	, m_y(vector.m_y)
	, m_z(vector.m_z)
{
}

Vector3 Vector3::LinearInterpolation(const Vector3& start, const Vector3& end, float delta)
{
	Vector3 v;
	v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
	v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
	v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);

	return v;
}

float Vector3::GetX() const
{
	return m_x;
}

float Vector3::GetY() const
{
	return m_y;
}

float Vector3::GetZ() const
{
	return m_z;
}

Vector3 Vector3::operator*(float scalar)
{
	return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
}

Vector3 Vector3::operator+(Vector3 vector)
{
	return Vector3(m_x + vector.m_x, m_y + vector.m_y, m_z + vector.m_z);
}
