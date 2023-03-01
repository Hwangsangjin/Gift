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

Vector4::Vector4(const Vector4& v)
	: m_x(v.m_x)
	, m_y(v.m_y)
	, m_z(v.m_z)
	, m_w(v.m_w)
{
}

Vector4::Vector4(const Vector3& v)
	: m_x(v.m_x)
	, m_y(v.m_y)
	, m_z(v.m_z)
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
