#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

Vector3 TransformComponent::GetPosition()
{
	return m_position;
}

Vector3 TransformComponent::GetRotation()
{
	return m_rotation;
}

Vector3 TransformComponent::GetScale()
{
	return m_scale;
}

void TransformComponent::SetPosition(const Vector3& position)
{
	m_position = position;
	UpdateWorldMatrix();
}

void TransformComponent::SetRotation(const Vector3& rotation)
{
	m_rotation = rotation;
	UpdateWorldMatrix();
}

void TransformComponent::SetScale(const Vector3& scale)
{
	m_scale = scale;
	UpdateWorldMatrix();
}

void TransformComponent::GetWorldMatrix(Matrix4x4& matrix) const
{
	matrix.SetMatrix(m_world_matrix);
}

void TransformComponent::SetBillboardMatrix(const Matrix4x4& matrix)
{
	m_world_matrix.m_matrix[0][0] = matrix.m_matrix[0][0];
	m_world_matrix.m_matrix[0][1] = matrix.m_matrix[0][1];
	m_world_matrix.m_matrix[0][2] = matrix.m_matrix[0][2];
	m_world_matrix.m_matrix[0][3] = matrix.m_matrix[0][3];

	m_world_matrix.m_matrix[1][0] = matrix.m_matrix[1][0];
	m_world_matrix.m_matrix[1][1] = matrix.m_matrix[1][1];
	m_world_matrix.m_matrix[1][2] = matrix.m_matrix[1][2];
	m_world_matrix.m_matrix[1][3] = matrix.m_matrix[1][3];

	m_world_matrix.m_matrix[2][0] = matrix.m_matrix[2][0];
	m_world_matrix.m_matrix[2][1] = matrix.m_matrix[2][1];
	m_world_matrix.m_matrix[2][2] = matrix.m_matrix[2][2];
	m_world_matrix.m_matrix[2][3] = matrix.m_matrix[2][3];
}

void TransformComponent::UpdateWorldMatrix()
{
	m_world_matrix.SetIdentity();

	Matrix4x4 temp;

	temp.SetIdentity();
	temp.SetScale(m_scale);
	m_world_matrix *= temp;

	temp.SetIdentity();
	temp.SetRotationX(m_rotation.m_x);
	m_world_matrix *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rotation.m_y);
	m_world_matrix *= temp;

	temp.SetIdentity();
	temp.SetRotationZ(m_rotation.m_z);
	m_world_matrix *= temp;

	temp.SetIdentity();
	temp.SetTranslation(m_position);
	m_world_matrix *= temp;
}
