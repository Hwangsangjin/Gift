#include "pch.h"
#include "Vector3.h"

Vector3::Vector3(float x, float y, float z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

Vector3::Vector3(const Vector3& v)
	: m_x(v.m_x)
	, m_y(v.m_y)
	, m_z(v.m_z)
{
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float delta)
{
	Vector3 v;

	v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
	v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
	v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);

	return v;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	Vector3 result;

	float length = std::sqrt(v.m_x * v.m_x) + (v.m_y * v.m_y) + (v.m_z * v.m_z);
	if (!length)
		return Vector3();

	result.m_x = v.m_x / length;
	result.m_y = v.m_y / length;
	result.m_z = v.m_z / length;

	return result;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.m_x = (v1.m_y * v2.m_z) - (v1.m_z * v2.m_y);
	result.m_y = (v1.m_z * v2.m_x) - (v1.m_x * v2.m_z);
	result.m_z = (v1.m_x * v2.m_y) - (v1.m_y * v2.m_x);

	return result;
}

Vector3 Vector3::operator*(float n) const
{
	return Vector3(m_x * n, m_y * n, m_z * n);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

Vector3& Vector3::operator+=(float n)
{
	m_x += n;
	m_y += n;
	m_z += n;

	return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;

	return *this;
}
