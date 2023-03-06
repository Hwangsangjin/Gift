  #include "pch.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

Matrix4x4::Matrix4x4()
{
	m_matrix[0][0] = 1.0f;
	m_matrix[1][1] = 1.0f;
	m_matrix[2][2] = 1.0f;
	m_matrix[3][3] = 1.0f;
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::SetIdentity()
{
	::memset(m_matrix, 0, sizeof(float) * 16);
	m_matrix[0][0] = 1.0f;
	m_matrix[1][1] = 1.0f;
	m_matrix[2][2] = 1.0f;
	m_matrix[3][3] = 1.0f;
}

void Matrix4x4::SetTranslation(const Vector3& translation)
{
	m_matrix[3][0] = translation.m_x;
	m_matrix[3][1] = translation.m_y;
	m_matrix[3][2] = translation.m_z;
}

void Matrix4x4::SetScale(const Vector3& scale)
{
	m_matrix[0][0] = scale.m_x;
	m_matrix[1][1] = scale.m_y;
	m_matrix[2][2] = scale.m_z;
}

void Matrix4x4::SetRotationX(float x)
{
	m_matrix[1][1] = std::cos(x);
	m_matrix[1][2] = std::sin(x);
	m_matrix[2][1] = -std::sin(x);
	m_matrix[2][2] = std::cos(x);
}

void Matrix4x4::SetRotationY(float y)
{
	m_matrix[0][0] = std::cos(y);
	m_matrix[0][2] = -std::sin(y);
	m_matrix[2][0] = std::sin(y);
	m_matrix[2][2] = std::cos(y);
}

void Matrix4x4::SetRotationZ(float z)
{
	m_matrix[0][0] = std::cos(z);
	m_matrix[0][1] = std::sin(z);
	m_matrix[1][0] = -std::sin(z);
	m_matrix[1][1] = std::cos(z);
}

void Matrix4x4::SetOrthographic(float width, float height, float near_plane, float far_plane)
{
	SetIdentity();
	m_matrix[0][0] = 2.0f / width;
	m_matrix[1][1] = 2.0f / height;
	m_matrix[2][2] = 1.0f / (far_plane - near_plane);
	m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
}

void Matrix4x4::SetPerspective(float fov, float aspect, float z_near, float z_far)
{
	float y_scale = 1.0f / std::tan(fov / 2.0f);
	float x_scale = y_scale / aspect;
	m_matrix[0][0] = x_scale;
	m_matrix[1][1] = y_scale;
	m_matrix[2][2] = z_far / (z_far - z_near);
	m_matrix[2][3] = 1.0f;
	m_matrix[3][2] = (-z_near * z_far) / (z_far - z_near);
	m_matrix[3][3] = 0.0f;
}

float Matrix4x4::GetDeterminant()
{
	Vector4 minor, v1, v2, v3;
	float det = 0.0f;

	v1 = Vector4(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], m_matrix[3][0]);
	v2 = Vector4(m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], m_matrix[3][1]);
	v3 = Vector4(m_matrix[0][2], m_matrix[1][2], m_matrix[2][2], m_matrix[3][2]);

	minor.Cross(v1, v2, v3);
	det = -(m_matrix[0][3] * minor.m_x + m_matrix[1][3] * minor.m_y + m_matrix[2][3] * minor.m_z + m_matrix[3][3] * minor.m_w);
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

	for (UINT i = 0; i < 4; i++)
	{
		for (UINT j = 0; j < 4; j++)
		{
			if (j != i)
			{
				unsigned int a = j;
				if (j > i)
					a -= 1;

				vec[a].m_x = m_matrix[j][0];
				vec[a].m_y = m_matrix[j][1];
				vec[a].m_z = m_matrix[j][2];
				vec[a].m_w = m_matrix[j][3];
			}
		}

		v.Cross(vec[0], vec[1], vec[2]);

		out.m_matrix[0][i] = static_cast<float>(std::pow(-1.0f, i)) * v.m_x / det;
		out.m_matrix[1][i] = static_cast<float>(std::pow(-1.0f, i)) * v.m_y / det;
		out.m_matrix[2][i] = static_cast<float>(std::pow(-1.0f, i)) * v.m_z / det;
		out.m_matrix[3][i] = static_cast<float>(std::pow(-1.0f, i)) * v.m_w / det;
	}

	SetMatrix(out);
}

Vector3 Matrix4x4::GetXDirection()
{
	return Vector3(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
}

Vector3 Matrix4x4::GetYDirection()
{
	return Vector3(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
}

Vector3 Matrix4x4::GetZDirection()
{
	return Vector3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
}

Vector3 Matrix4x4::GetTranslation()
{
	return Vector3(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}

void Matrix4x4::operator*=(const Matrix4x4& matrix)
{
	Matrix4x4 out;

	for (UINT i = 0; i < 4; i++)
	{
		for (UINT j = 0; j < 4; j++)
		{
			out.m_matrix[i][j] =
				m_matrix[i][0] * matrix.m_matrix[0][j] +
				m_matrix[i][1] * matrix.m_matrix[1][j] +
				m_matrix[i][2] * matrix.m_matrix[2][j] +
				m_matrix[i][3] * matrix.m_matrix[3][j];
		}
	}

	SetMatrix(out);
}

void Matrix4x4::SetMatrix(const Matrix4x4& matrix)
{
	::memcpy(m_matrix, matrix.m_matrix, sizeof(float) * 16);
}
