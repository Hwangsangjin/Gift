#include "pch.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Vector3.h"

Vector4::Vector4(float x, float y, float z, float w)
	: m_x(x)
	, m_y(y)
	, m_z(z)
	, m_w(w)
{
}

Vector4::Vector4(const Vector4& vector)
	: m_x(vector.m_x)
	, m_y(vector.m_y)
	, m_z(vector.m_z)
	, m_w(vector.m_w)
{
}

Vector4::Vector4(const Vector3& vector)
	: m_x(vector.GetX())
	, m_y(vector.GetY())
	, m_z(vector.GetZ())
	, m_w(1.0f)
{
}

void Vector4::Cross(Vector4& v1, Vector4& v2, Vector4 v3)
{
	m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) + v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
	m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
	m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
	m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
}

float Vector4::GetX() const
{
	return m_x;
}

float Vector4::GetY() const
{
	return m_y;
}

float Vector4::GetZ() const
{
	return m_z;
}

float Vector4::GetW() const
{
	return m_w;
}

void Vector4::SetX(const float& x)
{
	m_x = x;
}

void Vector4::SetY(const float& y)
{
	m_y = y;
}

void Vector4::SetZ(const float& z)
{
	m_z = z;
}

void Vector4::SetW(const float& w)
{
	m_w = w;
}
