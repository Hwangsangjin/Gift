#include "pch.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

void Matrix4x4::SetIdentity()
{
	::memset(m_mat, 0, sizeof(float) * 16);
	m_mat[0][0] = 1.0f;
	m_mat[1][1] = 1.0f;
	m_mat[2][2] = 1.0f;
	m_mat[3][3] = 1.0f;
}

void Matrix4x4::SetTranslation(const Vector3& translation)
{
	m_mat[3][0] = translation.GetX();
	m_mat[3][1] = translation.GetY();
	m_mat[3][2] = translation.GetZ();
}

void Matrix4x4::SetScale(const Vector3& scale)
{
	m_mat[0][0] = scale.GetX();
	m_mat[1][1] = scale.GetY();
	m_mat[2][2] = scale.GetZ();
}

void Matrix4x4::SetRotationX(float x)
{
	m_mat[1][1] = std::cos(x);
	m_mat[1][2] = std::sin(x);
	m_mat[2][1] = -std::sin(x);
	m_mat[2][2] = std::cos(x);
}

void Matrix4x4::SetRotationY(float y)
{
	m_mat[0][0] = std::cos(y);
	m_mat[0][2] = -std::sin(y);
	m_mat[2][0] = std::sin(y);
	m_mat[2][2] = std::cos(y);
}

void Matrix4x4::SetRotationZ(float z)
{
	m_mat[0][0] = std::cos(z);
	m_mat[0][1] = std::sin(z);
	m_mat[1][0] = -std::sin(z);
	m_mat[1][1] = std::cos(z);
}

void Matrix4x4::SetOrthographicProjection(float width, float height, float near_plane, float far_plane)
{
	SetIdentity();
	m_mat[0][0] = 2.0f / width;
	m_mat[1][1] = 2.0f / height;
	m_mat[2][2] = 1.0f / (far_plane - near_plane);
	m_mat[3][2] = -(near_plane / (far_plane - near_plane));
}

void Matrix4x4::SetPerspectiveProjection(float fov, float aspect, float z_near, float z_far)
{
	float y_scale = 1.0f / std::tan(fov / 2.0f);
	float x_scale = y_scale / aspect;
	m_mat[0][0] = x_scale;
	m_mat[1][1] = y_scale;
	m_mat[2][2] = z_far / (z_far - z_near);
	m_mat[2][3] = 1.0f;
	m_mat[3][2] = (-z_near * z_far) / (z_far - z_near);
}

float Matrix4x4::GetDeterminant()
{
	Vector4 minor, v1, v2, v3;
	float det;

	v1 = Vector4(m_mat[0][0], m_mat[1][0], m_mat[2][0], m_mat[3][0]);
	v2 = Vector4(m_mat[0][1], m_mat[1][1], m_mat[2][1], m_mat[3][1]);
	v3 = Vector4(m_mat[0][2], m_mat[1][2], m_mat[2][2], m_mat[3][2]);

	minor.Cross(v1, v2, v3);
	det = -(m_mat[0][3] * minor.GetX() + m_mat[1][3] * minor.GetY() + m_mat[2][3] * minor.GetZ() + m_mat[3][3] * minor.GetW());
	return det;
}

void Matrix4x4::Inverse()
{
	Matrix4x4 out;
	Vector4 v, vec[3];
	float det = 0.0f;

	det = GetDeterminant();
	if (!det)
		return;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (j != i)
			{
				unsigned int a = j;
				if (j > i)
				{
					a -= 1;
				}

				vec[a].SetX(m_mat[j][0]);
				vec[a].SetY(m_mat[j][1]);
				vec[a].SetZ(m_mat[j][2]);
				vec[a].SetW(m_mat[j][3]);
			}
		}

		v.Cross(vec[0], vec[1], vec[2]);

		out.m_mat[0][i] = static_cast<float>(std::pow(-1.0f, i)) * v.GetX() / det;
		out.m_mat[1][i] = static_cast<float>(std::pow(-1.0f, i)) * v.GetY() / det;
		out.m_mat[2][i] = static_cast<float>(std::pow(-1.0f, i)) * v.GetZ() / det;
		out.m_mat[3][i] = static_cast<float>(std::pow(-1.0f, i)) * v.GetW() / det;
	}

	SetMatrix(out);
}

Vector3 Matrix4x4::GetZDirection()
{
	return Vector3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
}

Vector3 Matrix4x4::GetXDirection()
{
	return Vector3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
}

Vector3 Matrix4x4::GetTranslation()
{
	return Vector3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
}

void Matrix4x4::operator*=(const Matrix4x4& matrix)
{
	Matrix4x4 out;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			out.m_mat[i][j] =
				m_mat[i][0] * matrix.m_mat[0][j] +
				m_mat[i][1] * matrix.m_mat[1][j] +
				m_mat[i][2] * matrix.m_mat[2][j] +
				m_mat[i][3] * matrix.m_mat[3][j];
		}
	}

	SetMatrix(out);
}

void Matrix4x4::SetMatrix(const Matrix4x4& matrix)
{
	::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
}
